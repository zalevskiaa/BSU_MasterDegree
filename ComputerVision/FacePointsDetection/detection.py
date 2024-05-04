import torch
from torch.utils.data import DataLoader
from torch import nn

# import pandas as pd
import numpy as np
# import matplotlib.pyplot as plt
import cv2
import os


def resize(img, label, res_size):
	"""
	:param: (original) img - (orig_h, orig_w, 3)-shaped rgb 0..255 image
	:param: (original) label - [x1, y1, ..., x7, y7] 0..orig_w(orig_h) array
	returns
	res_img - (h, w, 3)-shaped rgb 0..1 image
	res_label - [x1, y1, ..., x7, y7] 0..1 array
	"""
	assert len(img.shape) == 3
	assert label.shape == (28,)
	assert len(res_size) == 2

	res_img = cv2.resize(img, res_size[::-1])
	assert res_size == res_img.shape[:2]

	res_img = res_img / 255

	res_label = label.copy()
	h, w, _ = img.shape
	for i in range(0, 28, 2):
		res_label[i] /= w
		res_label[i + 1] /= h

	return res_img, np.array(res_label, dtype=float)


def unresize(img, label, orig_size):
	"""
	:param: img - (h, w, 3)-shaped rgb 0..1 image
	:param: label - [x1, y1, ..., x7, y7] 0..1 array
	returns
	res_img - (original) img - (orig_h, orig_w, 3)-shaped rgb 0..255 image
	res_label - (original) label - [x1, y1, ..., x7, y7] 0..orig_w(orig_h) array
	"""
	assert len(img.shape) == 3
	assert label.shape == (28,)
	assert len(orig_size) == 2

	res_img = cv2.resize(img, orig_size[::-1])
	assert orig_size == res_img.shape[:2]

	res_img = res_img * 255

	res_label = label.copy()
	h, w = orig_size
	for i in range(0, 28, 2):
		res_label[i] *= w
		res_label[i + 1] *= h

	return np.array(res_img, dtype=int), res_label


BATCH_IMG_SIZE = 100


class MyDataset(torch.utils.data.Dataset):
	def __init__(self, mode, train_gt, data_dir, fraction=0.8):
		assert mode in ('train', 'val')
		# assert mode == 'train'

		items_count = len(train_gt.items())

		# list of tuples: img_filename, img_orig_shape(h, w), img(100x100x3)[0..1], label[0..1]

		rng = range(0)

		if mode == 'train':
			rng = range(int(fraction * items_count))
		elif mode == 'val':
			rng = range(int(fraction * items_count), items_count)

		train_gt_items = list(train_gt.items())

		self.items = []
		for i in rng:
			img_filename, label = train_gt_items[i]

			img_path = data_dir + img_filename
			img = cv2.imread(img_path)[..., ::-1]
			assert img is not None
			assert len(img.shape) == 3

			img_orig_shape = img.shape[:2]

			assert label.shape == (28,)

			res_img, res_label = resize(img, label, (BATCH_IMG_SIZE, BATCH_IMG_SIZE))
			assert res_img.shape == (BATCH_IMG_SIZE, BATCH_IMG_SIZE, 3)
			assert res_label.shape == (28,)

			res_img = torch.from_numpy(res_img.transpose(2, 0, 1)).to(torch.float)
			res_label = torch.from_numpy(res_label).to(torch.float)
			assert res_img.shape == (3, BATCH_IMG_SIZE, BATCH_IMG_SIZE)
			assert res_label.shape == (28,)

			self.items.append((
				img_filename,
				img_orig_shape,
				res_img,
				res_label,
			))

	def __len__(self):
		return len(self.items)

	def __getitem__(self, index):
		return self.items[index]


class TestDataset(torch.utils.data.Dataset):
	def __init__(self, data_dir):
		# list of tuples: img_filename, img_orig_shape(h, w), img(3x100x100)[0..1]

		self.data_dir = data_dir

		filenames = os.listdir(data_dir)

		self.items = []
		for img_filename in filenames:

			self.items.append((
				img_filename,
			))

	def __len__(self):
		return len(self.items)

	def __getitem__(self, index):
		img_filename = self.items[index][0]

		img_path = self.data_dir + img_filename
		img = cv2.imread(img_path)[..., ::-1]
		assert img is not None
		assert len(img.shape) == 3

		img_orig_shape = img.shape[:2]

		label = np.zeros(28)
		assert label.shape == (28,)

		res_img, res_label = resize(img, label, (BATCH_IMG_SIZE, BATCH_IMG_SIZE))
		assert res_img.shape == (BATCH_IMG_SIZE, BATCH_IMG_SIZE, 3)

		res_img = torch.from_numpy(res_img.transpose(2, 0, 1)).to(torch.float)
		assert res_img.shape == (3, BATCH_IMG_SIZE, BATCH_IMG_SIZE)

		return img_filename, img_orig_shape, res_img


class MyModel(torch.nn.Module):
	def __init__(self, dropout=0.0):
		super(MyModel, self).__init__()
		self.conv1 = nn.Conv2d(3, 64, 3, padding=1)
		self.bn1 = nn.BatchNorm2d(64)
		self.relu1 = nn.ReLU()
		self.drop1 = nn.Dropout2d(dropout)
		self.maxpool1 = nn.MaxPool2d(2)
		self.conv2 = nn.Conv2d(64, 128, 3, padding=1)
		self.bn2 = nn.BatchNorm2d(128)
		self.relu2 = nn.ReLU()
		self.drop2 = nn.Dropout2d(dropout)
		self.maxpool2 = nn.MaxPool2d(2)
		self.conv3 = nn.Conv2d(128, 256, 3, padding=1)
		self.bn3 = nn.BatchNorm2d(256)
		self.relu3 = nn.ReLU()
		self.drop3 = nn.Dropout2d(dropout)
		self.maxpool3 = nn.MaxPool2d(2)
		self.lin4 = nn.Linear(36864, 64)
		self.bn4 = nn.BatchNorm1d(64)
		self.relu4 = nn.ReLU()
		self.drop4 = nn.Dropout(dropout)
		self.lin5 = nn.Linear(64, 28)

	def forward(self, batch):
		batch = self.maxpool1(self.drop1(self.relu1(self.bn1(self.conv1(batch)))))
		batch = self.maxpool2(self.drop2(self.relu2(self.bn2(self.conv2(batch)))))
		batch = self.maxpool3(self.drop3(self.relu3(self.bn3(self.conv3(batch)))))
		batch = torch.flatten(batch, 1)
		batch = self.drop4(self.relu4(self.bn4(self.lin4(batch))))
		batch = self.lin5(batch)
		return batch


device = torch.device('cpu')


def train(model, optimizer, dataloader, criterion, fast_train=False):
	model.train()
	losses_tr = []

	for _, _, images, labels in dataloader:
		images = images.to(device)
		labels = labels.to(device)

		optimizer.zero_grad()
		out = model(images)
		loss = criterion(BATCH_IMG_SIZE * out, BATCH_IMG_SIZE * labels)

		loss.backward()
		optimizer.step()
		losses_tr.append(loss.item())

		if fast_train:
			break

	return model, optimizer, np.mean(losses_tr)


def val(model, dataloader, criterion):
	model.eval()
	losses_val = []

	with torch.no_grad():
		for _, _, images, labels in dataloader:
			images = images.to(device)
			labels = labels.to(device)

			out = model(images)
			loss = criterion(BATCH_IMG_SIZE * out, BATCH_IMG_SIZE * labels)

			losses_val.append(loss.item())

	return np.mean(losses_val)


def test(model, dataloader):
	model.eval()

	result_labels = {}

	with torch.no_grad():
		for img_filenames, img_orig_shapes, images in dataloader:
			images = images.to(device)

			out = model(images)

			labels = out.cpu().detach().numpy()
			for label in range(len(labels)):
				for i in range(0, 28, 2):
					h, w = img_orig_shapes[0][label], img_orig_shapes[1][label],
					labels[label][i] *= w
					labels[label][i + 1] *= h

				result_labels[img_filenames[label]] = labels[label]
		# print(labels)
		# print(img_orig_shapes)
		# break

		# result_labels.extend(labels)

	return result_labels


def train_detector(train_gt, train_img_dir, fast_train=False):
	"""
	:param train_gt: {filename: label}
	:param train_img_dir: '.../train/images'
	:param fast_train:
	:return: model
	"""

	train_dataset = MyDataset('train', train_gt, train_img_dir.replace('\\', '/') + '/', fraction=0.9)
	# val_dataset = MyDataset('val', train_gt, train_img_dir.replace('\\', '/') + '/', fraction=0.9)

	BATCH_SIZE = 100

	train_dataloader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=False, num_workers=0)
	# val_dataloader = DataLoader(val_dataset, batch_size=BATCH_SIZE, shuffle=False, num_workers=0)

	EPOCHS = 1 if fast_train else 100

	model = MyModel(dropout=0.05).to(device)
	optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
	criterion = nn.MSELoss()

	# print(1)

	for epoch in range(EPOCHS):
		model, optimizer, loss_tr = train(model, optimizer, train_dataloader, criterion, fast_train=True)
		# loss_val = val(model, val_dataloader, criterion)

	# print(2)

	return model

def detect(model_filename, test_img_dir):
	"""
	:param model_filename:
	:param test_img_dir:
	:return: detected_points = {filename: np.array([x1, y1, ..., x14, y14])}
	"""
	model = MyModel(dropout=0.05).to(device)
	model.load_state_dict(torch.load(model_filename, map_location=device))
	model.eval()

	# print(3)

	BATCH_SIZE = 100

	test_dataset = TestDataset(test_img_dir.replace('\\', '/') + '/')
	test_dataloader = DataLoader(test_dataset, batch_size=BATCH_SIZE, shuffle=False, num_workers=0)

	# print(4)

	res_labels = test(model, test_dataloader)

	# print(5)

	return res_labels