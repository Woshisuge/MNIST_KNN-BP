#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cmath>
#include <time.h>
#include"head.h"

#pragma warning(disable:4996)
using namespace std;
extern float success_rate_;
extern int test_size_list;//ȫ�ֱ���
const int first = 784;//784��������
const int second = 100;//100��������
const int third = 10;//10�������
const double alpha = 0.35;//ѧϰЧ�ʲ���

int input[first];//����
int target[third];//�������
double weight1[first][second];//�����-�������Ȩֵ
double weight2[second][third];//������-������Ȩֵ
double output1[second];//���ز�����
double output2[third];//���������
double delta1[second];//������-���������
double delta2[third];//�����-����������
double b1[second];//�����-���������ֵ
double b2[third];//������-�������ֵ

double test_num = 0.0;
double test_success_count = 0.0;

double f_(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

//���ز���������
void op1_()
{
	for (int j = 0; j < second; j++)
	{
		double sigma = 0;
		for (int i = 0; i < first; i++)
		{
			sigma += input[i] * weight1[i][j];
		}
		double x = sigma + b1[j];//���ز������
		output1[j] = f_(x);//���ز�����
	}
}

//�������������
void op2_()
{
	for (int k = 0; k < third; k++)
	{
		double sigma = 0;
		for (int j = 0; j < second; j++)
		{
			sigma += output1[j] * weight2[j][k];
		}
		double x = sigma + b2[k];//����������
		output2[k] = f_(x);//���������
	}
}

//����������
void dt2_()
{
	for (int k = 0; k < third; k++)
	{
		delta2[k] = (output2[k]) * (1.0 - output2[k]) * (output2[k] - target[k]);
	}
}

//���ز������
void dt1_() 
{
	for (int j = 0; j < second; j++) 
	{
		double sigma = 0;
		for (int k = 0; k < third; k++) 
		{
			sigma += weight2[j][k] * delta2[k];//��������ļ�Ȩ�ܺ�
		}
		delta1[j] = (output1[j]) * (1.0 - output1[j]) * sigma;
	}
}

//���ز�ķ����������
void feedback_second() {
	for (int j = 0; j < second; j++) 
	{
		b1[j] = b1[j] - alpha * delta1[j];//������ֵ
		for (int i = 0; i < first; i++) 
		{
			weight1[i][j] = weight1[i][j] - alpha * input[i] * delta1[j];//����Ȩֵ
		}
	}
}

//�����ķ����������
void feedback_third() {
	for (int k = 0; k < third; k++) 
	{
		b2[k] = b2[k] - alpha * delta2[k];//������ֵ
		for (int j = 0; j < second; j++) 
		{
			weight2[j][k] = weight2[j][k] - alpha * output1[j] * delta2[k];//����Ȩֵ
		}
	}
}

//��ʼ������
void initialize() 
{
	srand((int)time(0) + rand());
	for (int i = 0; i < first; i++) 
	{
		for (int j = 0; j < second; j++) 
		{
			weight1[i][j] = rand() % 1000 * 0.001 - 0.5;//��������������Ȩֵ�������ֵ�ĸ�ֵ
		}
	}
	for (int j = 0; j < second; j++) 
	{
		for (int k = 0; k < third; k++) 
		{
			weight2[j][k] = rand() % 1000 * 0.001 - 0.5;//��������������Ȩֵ�������ֵ�ĸ�ֵ
		}
	}
	for (int j = 0; j < second; j++) 
	{
		b1[j] = rand() % 1000 * 0.001 - 0.5;//����������������ֵ�������ֵ�ĸ�ֵ
	}
	for (int k = 0; k < third; k++) 
	{
		b2[k] = rand() % 1000 * 0.001 - 0.5;//����������������ֵ�������ֵ�ĸ�ֵ
	}
}

//ѵ������
void training() 
{
	FILE *image_train;
	FILE *image_label;
	image_train = fopen("E:/mnist/mnist_train/train-images.idx3-ubyte", "rb");//��ѵ��ͼƬ���ݼ�
	image_label = fopen("E:/mnist/mnist_train/train-labels.idx1-ubyte", "rb");//��ѵ����ǩ���ݼ�
	if (image_train == NULL || image_label == NULL) 
	{
		cout << "can't open the file!" << endl;
		exit(0);
	}

	unsigned char image_buf[784];
	unsigned char label_buf[10];

	int useless[1000];
	fread(useless, 1, 16, image_train);//ȥ�����ݼ�ǰ16���ֽ�
	fread(useless, 1, 8, image_label);//ȥ�����ݼ�ǰ8���ֽ�

	int cnt = 0;//������ѵ��ͼƬ��Ŀ
	cout << "Start training..." << endl;
	while (!feof(image_train) && !feof(image_label))
	{
		memset(image_buf, 0, 784);//ÿ�ν���ѭ����image_buf��������
		memset(label_buf, 0, 10);//ÿ�ν���ѭ����label_buf��������
		fread(image_buf, 1, 784, image_train);//��image_train��ǰ784���ֽڶ��뵽image_buf��
		fread(label_buf, 1, 1, image_label);//��image_label��ǰ1���ֽڶ��뵽label_buf��

		//��ʼ������������
		for (int i = 0; i < 784; i++) 
		{
			if ((unsigned int)image_buf[i] < 128) 
				input[i] = 0;
			else 
				input[i] = 1;
		}

		//��ʼ���������������
		int target_value = (unsigned int)label_buf[0];
		for (int k = 0; k < third; k++) {
			target[k] = 0;
		}
		target[target_value] = 1;

		//���ϵ��������Ȩֵ����ֵ
		op1_();
		op2_();
		dt2_();
		dt1_();
		feedback_second();
		feedback_third();

		cnt++;
		//if (cnt % 1000 == 0) {
		//	cout << "training image: " << cnt << endl;
		//}
		//if (cnt ==2000) break;//������cnt��ͼƬ������ѭ��
	}
	cout << endl;
}

//���Ժ���
void testing()
{
	FILE *image_test;
	FILE *image_test_label;
	image_test = fopen("E:\\mnist\\t10k-images-idx3-ubyte", "rb+");
	image_test_label = fopen("E:\\mnist\\t10k-labels-idx1-ubyte", "rb+");
	if (image_test == NULL || image_test_label == NULL) {
		cout << "can't open the file!" << endl;
		exit(0);
	}

	unsigned char image_buf[784];
	unsigned char label_buf[10];

	int useless[1000];
	fread(useless, 1, 16, image_test);
	fread(useless, 1, 8, image_test_label);

	while (test_size_list)
	{
		test_size_list--;
		memset(image_buf, 0, 784);
		memset(label_buf, 0, 10);
		fread(image_buf, 1, 784, image_test);
		fread(label_buf, 1, 1, image_test_label);

		//��ʼ������������
		for (int i = 0; i < 784; i++) {
			if ((unsigned int)image_buf[i] < 128) {
				input[i] = 0;
			}
			else {
				input[i] = 1;
			}
		}
		
		//��0/1����ͼƬ��ʽ���
		int n = 0;
		for (int i = 0; i < 784; i++)
		{
			
			if (n == 28)
			{
				/*cout << endl;*/
				n = 0;
			}				
			/*cout << input[i];*/
			n++;
		}

		//��ʼ���������������
		for (int k = 0; k < third; k++) {
			target[k] = 0;
		}
		int target_value = (unsigned int)label_buf[0];
		target[target_value] = 1;

		//��ʼ��������ʵ�����
		op1_();
		op2_();

		//ѡ�����������ֵ
		double max_value = -99999;
		int max_index = 0;
		int k;
		for (k = 0; k < third; k++) 
		{
			if (output2[k] > max_value) 
			{
				max_value = output2[k];
				max_index = k;
			}
		}

		//ʶ������ʾ
		if (target[max_index] == 1)
		{
			test_success_count++;
			//cout << "ʶ������" << max_index <<endl;
		}
		//else
		//	cout << "ʶ������" << max_index << endl;
		test_num++;

	}
	//cout << endl;
	//cout << "The success rate: " << test_success_count / test_num << endl;
	success_rate_ = test_success_count / test_num;

}
