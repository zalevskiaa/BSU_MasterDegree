import os
import math
import cv2

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# from tqdm.notebook import tqdm

import torch
import torch.nn as nn
from torch.nn import functional as F
# from torch.nn.modules.loss import _WeightedLoss
# from torch.autograd import Variable
from torch.utils.data import Dataset, DataLoader
# from torch.utils.data.sampler import SubsetRandomSampler
# from torch.utils.data import TensorDataset, random_split
import torchvision
# from torchvision import datasets, transforms, models
from torchvision.models import resnet50, ResNet50_Weights

device = torch.device("cpu")


LABEL_SIZE = 50
def label_preprocess(label):
	"""
	0,...,49 -> torch[..., 0, 1, 0, ...]
	"""
	assert isinstance(label, int) and 0 <= label < LABEL_SIZE

	res_label = torch.zeros(LABEL_SIZE, dtype=torch.float)
	res_label[label] = 1

	return res_label


from PIL import Image

preprocess = torchvision.transforms.Compose([
	torchvision.transforms.Resize(256),
	torchvision.transforms.CenterCrop(224),
	torchvision.transforms.ToTensor(),
	# torchvision.transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])

augmentation = torchvision.transforms.Compose([
	torchvision.transforms.Resize(256),
	torchvision.transforms.CenterCrop(224),
	torchvision.transforms.ToTensor(),

	torchvision.transforms.RandomPerspective(distortion_scale=0.5, p=0.7),

	# torchvision.transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])


def img_read(img_path):
	input_image = Image.open(img_path)
	input_image = input_image.convert(mode='RGB')
	return input_image


def img_preprocess(img, aug=False):
	prep_image = augmentation(img) if aug else preprocess(img)
	# assert prep_image.shape[0] == 3, f'lol shape {img_path} is {prep_image.shape}'
	return prep_image


def read_and_preprocess(img_path, aug=False):
	input_image = img_read(img_path)
	prep_image = img_preprocess(input_image, aug=aug)
	return prep_image


class MyDataset(Dataset):
	def __init__(self, train_gt, imgs_dir, aug=False):
		self.imgs_dir = imgs_dir
		self.aug = aug

		self.items = []
		for img_filename, label in train_gt.items():
			self.items.append((img_filename, label))

	def __len__(self):
		return len(self.items)

	def __getitem__(self, index):
		img_filename, label = self.items[index]

		prep_img = read_and_preprocess(self.imgs_dir + img_filename, aug=self.aug)
		prep_label = label_preprocess(label)
		return prep_img, prep_label

class MyFastDataset(Dataset):
	def __init__(self, train_gt, imgs_dir, aug=False):
		self.imgs_dir = imgs_dir
		self.aug = aug

		self.items = []
		for img_filename, label in train_gt.items():
			img_path = self.imgs_dir + img_filename

			input_image = img_read(img_path)
			# prep_img = read_and_preprocess(, aug=self.aug)
			prep_label = label_preprocess(label)

			self.items.append((input_image, prep_label))

	def __len__(self):
		return len(self.items)

	def __getitem__(self, index):
		input_image, prep_label = self.items[index]
		prep_image = img_preprocess(input_image, aug=self.aug)
		return prep_image, prep_label


class MyModel(torch.nn.Module):
	def __init__(self):
		super(MyModel, self).__init__()

		# torch.hub.set_dir('test_cache/')
		self.model = resnet50(weights=None)

		for param in self.model.parameters():
			param.requires_grad = False

		self.model.fc = nn.Sequential(
			nn.Linear(in_features=2048, out_features=50),
		)

	def forward(self, x):
		return F.log_softmax(self.model(x), dim=1)


def count_correct(a, b):
	"""
	a, b: (batch_size, labels_count)-sized tensors
	returns: (correct, total)
	"""
	assert a.shape == b.shape
	batch_size = a.shape[0]
	am = a.argmax(dim=1)
	bm = b.argmax(dim=1)

	assert am.shape[0] == bm.shape[0] == batch_size
	correct = int((am == bm).sum())

	return correct, batch_size


def train(model, optimizer, dataloader, criterion, fast_train=False):
	model.train()

	losses_tr = []
	correct, total = 0, 0

	for images, labels in dataloader:
		images = images.to(device)
		labels = labels.to(device)

		optimizer.zero_grad()
		out = model(images)
		loss = criterion(out, labels)
		loss.backward()
		optimizer.step()

		losses_tr.append(loss.item())
		c, t = count_correct(out, labels)
		correct += c
		total += t

		if fast_train:
			break

	return model, optimizer, np.mean(losses_tr), correct, total


def val(model, dataloader, criterion):
	model.eval()

	losses_val = []
	correct, total = 0, 0

	with torch.no_grad():
		for images, labels in dataloader:
			images = images.to(device)
			labels = labels.to(device)

			out = model(images)
			loss = criterion(out, labels)

			losses_val.append(loss.item())
			c, t = count_correct(out, labels)
			correct += c
			total += t

	return np.mean(losses_val), correct, total


def set_rg(model, rg_true, abort=False):
	if abort:
		return

	p_total = 0
	for i, e in enumerate(model.parameters()):
		e.requires_grad = False
		p_total += 1

	assert 0 <= rg_true <= p_total
	first_rgt_param = p_total - rg_true

	rgt = 0
	for i, e in enumerate(model.parameters()):
		if i >= first_rgt_param:
			e.requires_grad = True
			rgt += 1

	assert rgt == rg_true


class MyTestDataset(Dataset):
	def __init__(self, imgs_dir):
		self.imgs_dir = imgs_dir
		filenames = os.listdir(imgs_dir)
		self.items = [filename for filename in filenames]

	def __len__(self):
		return len(self.items)

	def __getitem__(self, index):
		img_filename = self.items[index]
		prep_img = read_and_preprocess(self.imgs_dir + img_filename, aug=False)
		return img_filename, prep_img


def test(model, dataloader):
	model.eval()

	result_labels = {}

	with torch.no_grad():
		for img_filenames, images in dataloader:
			images = images.to(device)

			out = model(images)

			labels = out.cpu().detach().numpy()
			for filename, label in zip(img_filenames, labels):
				am = label.argmax()
				result_labels[filename] = am
			# break

	return result_labels


def train_classifier(train_gt, train_img_dir, fast_train=False):
	train_dataset = MyDataset(train_gt, train_img_dir.replace('\\', '/') + '/', aug=True)
	# val_dataset = MyDataset(train_gt_v, paths.train_images_dir, aug=False)

	print(0)

	BATCH_SIZE = 50

	train_dataloader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=False, num_workers=0)
	# val_dataloader = DataLoader(val_dataset, batch_size=BATCH_SIZE, shuffle=False, num_workers=0)

	EPOCHS = 1 if fast_train else 20

	print(1)

	model = MyModel().to(device)

	print(2)

	optimizer = torch.optim.Adam(model.parameters(), lr=0.0002)
	scheduler = torch.optim.lr_scheduler.ExponentialLR(optimizer, gamma=0.8)
	criterion = nn.CrossEntropyLoss()

	print(3)

	for epoch in range(EPOCHS):
		if True:
			if epoch == 5:
				set_rg(model, 6)
			elif epoch == 8:
				set_rg(model, 10)
			elif epoch == 10:
				set_rg(model, 20)
			elif epoch == 13:
				set_rg(model, 50)
			elif epoch == 15:
				set_rg(model, 161)

		model, optimizer, loss_tr, ct, tt = train(model, optimizer, train_dataloader, criterion, fast_train=fast_train)
		# loss_val, cv, tv = val(model, val_dataloader, criterion)
		scheduler.step()

	print(4)

	return model


def classify(model_path, test_img_dir):
	print(5)

	model = MyModel().to(device)
	model.load_state_dict(torch.load(model_path, map_location=device))
	model.eval()

	print(6)

	BATCH_SIZE = 50

	test_dataset = MyTestDataset(test_img_dir.replace('\\', '/') + '/')
	test_dataloader = DataLoader(test_dataset, batch_size=BATCH_SIZE, shuffle=False, num_workers=0)
	res_labels = test(model, test_dataloader)

	print(7)

	return res_labels
