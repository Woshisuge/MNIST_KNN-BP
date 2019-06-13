#include "head.h"
#include<ctime>
void initialize();
void training();
void testing();
int test_size_list;//ȫ�ֱ���
float success_rate_;
int main()
{
	Image2BinaryData IBD(28, 28);											//����ͼƬ��С(Height,Width)
	clock_t time1, time2,time3,time4;
	cout << "----------���ɲ��Լ��ļ�-------------\n" << endl;
	string testfilefolder = "E:\\mnist\\100mnist";		//����ͼƬ�ļ�·��
	vector<string> testfileLists = IBD.getFileLists(testfilefolder);			//����ļ����б�

	test_size_list = testfileLists.size();
	cout << "Images Number: " << test_size_list << endl;									//����ļ�����
	string testimagebinfilepath = "E:\\mnist\\t10k-images-idx3-ubyte";		//����ͼƬת������·��
	string testlabelbinfilepath = "E:\\mnist\\t10k-labels-idx1-ubyte";		//���Ա�ǩת������·��
	vector<cv::Mat> TestImagesMat;															//�����洢����ͼƬ����ֵ
	vector<int> test_image_labels(test_size_list);											//�����洢�������ǩ�б�
	IBD.ReadImage(testfilefolder, testfileLists, test_image_labels, TestImagesMat);			//��ȡ����ͼƬ
	IBD.Image2BinaryFile(testimagebinfilepath, TestImagesMat, test_image_labels);			//����ͼƬת�������ļ�
	IBD.Label2BinaryFile(testlabelbinfilepath, test_image_labels);							//���Ա�ǩת�������ļ�
	
	cout << "��ʼ���ԣ�" << endl;
	time1 = clock();
	initialize();
	time2 = clock();
	cout << "��ʼ��ʱ�䣺" << (double)(time2 - time1)/ CLOCKS_PER_SEC << 's' << endl;
	training();
	time3 = clock();
	cout << "ѵ��ʱ�䣺" << (double)(time3 - time2) / CLOCKS_PER_SEC << 's' << endl;
	testing();
	time4 = clock();
	cout << "����ʱ�䣺" << (double)(time4 - time3) / CLOCKS_PER_SEC << 's' << endl;
	cout << "׼ȷ�ʣ�" << success_rate_ << endl;
	cout << "������ʱ��" << (double)(time4 - time1) / CLOCKS_PER_SEC <<'s'<< endl;
	system("pause");
	return 0;
}