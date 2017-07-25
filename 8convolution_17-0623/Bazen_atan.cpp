#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //�����X�g���[��
#include<math.h>
#include<time.h>//���Ԃ�p����
#include <direct.h>//�t�H���_���쐬��
#include<stdio.h>
#include <windows.h>
#include <vector>

#include <opencv2/opencv.hpp>

#define PI 3.14159265

using namespace std;

//�������m�ۂ��s�����߂̃w�b�_
#define ANSI				
#include "nrutil.h"	

int Bazen_kernel(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);//kernel��p����Bazen�̎�@
int Bazen_s(int &image_y,int &image_x,double *Gxx[],double *Gyy[],double *Gxy[],double *AngleF[],double *Angle[]);//Bazen�̎�@�Ŋp�x�����߂�
int Bazen(char image_nameP2[],int &image_x,int &image_y,int paramerter[],int paramerter_count,int sd,char date[],char date_directory[]);//�ׂ荇��pixel�ŋP�x�ω������߂�D�{����Bazen�̕��@


int Bazen_kernel(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]){
	
//////////////////��`///////////////////////////////////////////////////////////////////////////////////////////
	
	char *Input_Filename_Bazen_atan1_s = "\\V(0).csv";			//���̓t�@�C�����̎w��
	char *Input_Filename_Bazen_atan3_s = "\\V(90).csv";			//���̓t�@�C�����̎w��
	char inputdate_directory_Bazen_atan[128];					//inputdate�̃t�H���_�̃f�B���N�g��

	char date_directory4[128];						//�o�̓t�H���_�̃f�B���N�g��

	//�o�͌��ʂ̃t�@�C�����̎w��
	char *math_Bazen_atan1_s = "\\Bazen_arctan.csv";
	char *math_Bazen_atan5_s = "\\threshold_high.csv";
	char *math_Bazen_atan8_s = "\\threshold2.csv";
	char *math_Bazen_atan9_s = "\\threshold_low.csv";

	//���̓t�@�C���̃f�B���N�g��
	char Input_Filename_Bazen_atan1[255];						
	char Input_Filename_Bazen_atan3[255];

	//�o�̓t�@�C���̃f�B���N�g��
	char math_Bazen_atan1[128];							//����						
	char math_Bazen_atan5[128];							//threshold_high
	char math_Bazen_atan8[128];							//2��臒l�����
	char math_Bazen_atan9[128];							//threshold(use_Rvector_flag�̉����d��V�̑傫���j
	
	double threshold_high;
	double threshold_low;
	double threshold_low_abs;
	double threshold_high_abs;

	FILE *fp_arctan,*fp_threshold_low,*fp_threshold_high,*fp_threshold2;

	printf("****************************************\n");
	printf("start�F Bazen_atan\n");
	printf("****************************************\n");

	int i=0,j=0;
/////////////////////////////////�����ݒ�1 : input�ݒ�//////////////////////////////////////////////////////////////////

	//Nrutil��p�����������̊m��
	double **V0 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V90 = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_atan_high_flag = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_atan_low_flag = matrix(0, image_x - 1, 0, image_y - 1);
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	double **AngleF = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gxx = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gyy = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gxy = matrix(0, image_x - 1, 0, image_y - 1);

	//������
	for (i = 0; i < image_y; i++) {
		for (j = 0; j < image_x; j++) {
			V0[j][i] = 0;
			V90[j][i] = 0;
			threshold_atan_high_flag[j][i] = 0;
			threshold_atan_low_flag[j][i] = 0;
			Angle[j][i] = 0;
			AngleF[j][i] = 0;
			Gxx[j][i] = 0;
			Gyy[j][i] = 0;
			Gxy[j][i] = 0;
		}
	}

////////////////////////////���o�̓f�B���N�g���̍쐬//////////////////////////////////////////////////////////////////////////////

	if(paramerter[0]==1 || paramerter[0]==2){
		sprintf(inputdate_directory_Bazen_atan, "%s%d�~%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count],paramerter[paramerter_count], sd);
			}else{
		sprintf(inputdate_directory_Bazen_atan, "%s%dk_conv_sd%d", date_directory, paramerter[paramerter_count], sd);
	}
	
	//Input�t�@�C���̐ݒ�
	sprintf(Input_Filename_Bazen_atan1, "%s%s", inputdate_directory_Bazen_atan, Input_Filename_Bazen_atan1_s);
	sprintf(Input_Filename_Bazen_atan3, "%s%s", inputdate_directory_Bazen_atan, Input_Filename_Bazen_atan3_s);
	printf("Input_Filename_atan1=%s\n", Input_Filename_Bazen_atan1);

	//Output�f�B���N�g���̐ݒ�
	if(paramerter[0]==1 || paramerter[0]==2){
		sprintf(date_directory4, "%s%d�~%dsobel_Bazen_atan_sd%d", date_directory, paramerter[paramerter_count],paramerter[paramerter_count], sd);
	}else{
		sprintf(date_directory4, "%s%dk_Bazen_atan_sd%d", date_directory, paramerter[paramerter_count], sd);
	}

	//Output�t�H���_�̃f�B���N�g���쐬
	if (_mkdir(date_directory4) == 0) {
		printf("�t�H���_ %s ���쐬���܂���\n", date_directory4);
	}
	else {
		printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
	}

	//Output�t�@�C���̃f�B���N�g���ݒ�
	sprintf(math_Bazen_atan1, "%s%s", date_directory4, math_Bazen_atan1_s);
	sprintf(math_Bazen_atan5, "%s%s", date_directory4, math_Bazen_atan5_s);
	sprintf(math_Bazen_atan8, "%s%s", date_directory4, math_Bazen_atan8_s);
	sprintf(math_Bazen_atan9, "%s%s", date_directory4, math_Bazen_atan9_s);

	//Input�t�@�C�����J��
	ifstream V_0(Input_Filename_Bazen_atan1);
	ifstream V_90(Input_Filename_Bazen_atan3);

	if(!V_0){printf("\n���̓G���[ V(0).csv������܂���\nInput_Filename_Bazen_atan1=%s",Input_Filename_Bazen_atan1);return 1;}
	if(!V_90){printf("\n���̓G���[ V(90).csv������܂���\nInput_Filename_Bazen_atan3=%s",Input_Filename_Bazen_atan3);return 1;}
	
///////////Input�t�@�C���̓ǂݍ���//////////////////////////////////////////////////////////////////////////////////
	int count_small=0,count_large=0;

	i = 0;
	string str_0, str_90;
	count_large = 0;			//������
	while (getline(V_0, str_0)) {					//���̃��[�v���ł��ׂĂ��s��
		count_small = 0;		//������

		//���낢���`�D
		string token_V_0;
		istringstream stream_V_0(str_0);

		getline(V_90, str_90);
		string token_V_90;
		istringstream stream_V_90(str_90);

		//�z��ɑ��
		while (getline(stream_V_0, token_V_0, ',')) {	 //��s�ǂݎ��DV0�̂݁C�J��Ԃ��͈͎̔w��ɗp����

			double tmp_V_0 = stof(token_V_0);			//�����𐔎��ɕϊ�
			V0[count_small][count_large] = tmp_V_0;		//�z��ɑ��

			getline(stream_V_90, token_V_90, ',');
			double tmp_V_90 = stof(token_V_90);
			V90[count_small][count_large] = tmp_V_90;

			++count_small;
		}
		++count_large;
	}

/////////////////�召����(臒l�����߂邽��)/////////////////////////////////////////////////////////////
		for (count_small = 0; count_small < image_x; ++count_small) {
			for (count_large = 0; count_large < image_y; ++count_large) {

				for (count_small = 0; count_small < image_x; ++count_small) {
					for (count_large = 0; count_large < image_y; ++count_large) {
						if (V0[count_small][count_large] >= V90[count_small][count_large]) {
							threshold_atan_high_flag[count_small][count_large] = 1;			//V�̑召��r
							threshold_atan_low_flag[count_small][count_large] = 3;
						}else{
							threshold_atan_high_flag[count_small][count_large] = 3;
							threshold_atan_low_flag[count_small][count_large] = 1;
						}
					}
				}
			}
		}

////////////////////////////Bazen�̌v�Z///////////////////////////////////////////////////////////////////////
		//�X����2��
		for(j=0;j<image_y;++j){
			for(i=1;i<image_x;++i){
				Gxx[i][j]=pow(V0[i][j],2);
				Gyy[i][j]=pow(V90[i][j],2);
				Gxy[i][j]=V0[i][j]*V90[i][j];
			}
		}
		//Angle�����߂�
		Bazen_s(image_y,image_x,Gxx,Gyy,Gxy,AngleF,Angle);

/////////////////////////�o�̓t�@�C�����J��///////////////////////////////////////////////////////////////////////////////////
	if((fp_arctan=fopen(math_Bazen_atan1,"w"))==NULL){cout<<"���̓G���[ Bazen_arctan.csv���J���܂���";exit(1);}
	if((fp_threshold_high=fopen(math_Bazen_atan5,"w"))==NULL){cout<<"���̓G���[ threshold_high.csv���J���܂���";exit(1);}
	if((fp_threshold2=fopen(math_Bazen_atan8,"w"))==NULL){cout<<"���̓G���[ threshold2.csv���J���܂���";exit(1);}
	if((fp_threshold_low=fopen(math_Bazen_atan9,"w"))==NULL){cout<<"���̓G���[ threshold.csv���J���܂���";exit(1);}
	
	//�t�@�C���ւ̏�������
	for(j=0;j<image_y;++j){
		for(i=0;i<image_x;++i){
	
		//�p�x
		fprintf(fp_arctan,"%lf,",Angle[i][j]);
		if(i==image_x - 1){fprintf(fp_arctan,"\n");}

		//���臒l
		if(threshold_atan_high_flag[i][j]==1){threshold_high=V0[i][j];fprintf(fp_threshold_high,"%lf,",threshold_high);}
		if(threshold_atan_high_flag[i][j]==3){threshold_high=V90[i][j];fprintf(fp_threshold_high,"%lf,",threshold_high);}
		if(i==image_x - 1){fprintf(fp_threshold_high,"\n");}

		//����臒l
		if(threshold_atan_low_flag[i][j]==1){threshold_low=V0[i][j];fprintf(fp_threshold_low,"%lf,",threshold_low);}
		if(threshold_atan_low_flag[i][j]==3){threshold_low=V90[i][j];fprintf(fp_threshold_low,"%lf,",threshold_low);}
		if(i==image_x - 1){fprintf(fp_threshold_low,"\n");}

		threshold_high_abs=threshold_high;
		threshold_low_abs=threshold_low;

		//臒l�̑傫�������Βl�łƂ�
		if(threshold_high<0){threshold_high_abs=threshold_high_abs*-1;}
		if(threshold_low<0){threshold_low_abs=threshold_low_abs*-1;}
		if(threshold_high_abs>threshold_low_abs){
			fprintf(fp_threshold2,"%lf,",threshold_high);
		}else{
			fprintf(fp_threshold2,"%lf,",threshold_low);
		}
		if(i==image_x - 1){fprintf(fp_threshold2,"\n");}
		//if(V0[j]<0)Angle[j]=Angle[j]-180;
		}
	}
///////////////////////////�������ݏI���/////////////////////////////////////////////////////////			
		
	//�t�@�C�������
	fclose(fp_arctan);
	fclose(fp_threshold_high);
	fclose(fp_threshold2);
	fclose(fp_threshold_low);
	
////////////////////////log�t�@�C���̍쐬//////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];

	sprintf(filename_log, "%s\\log.txt",date_directory4);	//log�t�@�C���쐬�̃f�B���N�g���w��
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("log�t�@�C�����J���܂���");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date);						//����
	fprintf(fp_date,"�g�p�f�[�^ : %s\n",inputdate_directory_Bazen_atan);		//�g�p������ݍ��ݍς݃f�[�^
	fprintf(fp_date,"�ۑ���     : %s\n",date_directory);			//�ۑ���
	fclose(fp_date);

	printf("�g�p�f�[�^ : %s\n",inputdate_directory_Bazen_atan);				//�g�p������ݍ��ݍς݃f�[�^
	printf("log�t�@�C�� %s ��ۑ����܂���\n",filename_log);

	//�������̊J��
	free_matrix(V0, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V90, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_atan_high_flag, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_atan_low_flag, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gxx, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gyy, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gxy, 0, image_x - 1, 0, image_y - 1);

	printf("finish : Bazen_kernel\n");

	return 0;
}

int Bazen_s(int &image_y,int &image_x,double *Gxx[],double *Gyy[],double *Gxy[],double *AngleF[],double *Angle[]){
	//Bazen�̕��@�ɂ��p�x����

	//Nrutil��p�����������̊m��
	double **sGxx = matrix(0, image_x - 1, 0, image_y - 1);
	double **sGyy = matrix(0, image_x - 1, 0, image_y - 1);
	double **sGxy = matrix(0, image_x - 1, 0, image_y - 1);

	//������
	for (int i = 0; i < image_y; i++) {
		for (int j = 0; j < image_x; j++) {
			sGxx[j][i] = 0;
			sGyy[j][i] = 0;
			sGxy[j][i] = 0;
		}
	}

	//16�~16�̗̈�ł̘a
		for(int j=0;j<image_y;++j){
			for(int i=0;i<image_x;++i){
				for(int l=j-7;l<j+8+1;++l){
					for(int k=i-7;k<i+8+1;++k){
						if(l>=0 && k>=0 && l<image_x && k<image_y){	//�̈悪�摜�O�ɂ͂ݏo��ꍇ��0�Ƃ��Ęa���s��
							sGxx[i][j]=sGxx[i][j]+Gxx[k][l];
							sGyy[i][j]=sGyy[i][j]+Gyy[k][l];
							sGxy[i][j]=sGxy[i][j]+Gxy[k][l];
						}
					}
				}
			}
		}
		
		for(int j=0;j<image_y;++j){
			for(int i=0;i<image_x;++i){
				if(sGxx[i][j]-sGyy[i][j]>=0)AngleF[i][j]=(atan((2*sGxy[i][j])/(sGxx[i][j]-sGyy[i][j]))/2);
				if(sGxx[i][j]-sGyy[i][j]<0 && sGxy[i][j]>=0)AngleF[i][j]=((atan((2*sGxy[i][j])/(sGxx[i][j]-sGyy[i][j]))/2)+PI);
				if(sGxx[i][j]-sGyy[i][j]<0 && sGxy[i][j]<0)AngleF[i][j]=((atan((2*sGxy[i][j])/(sGxx[i][j]-sGyy[i][j]))/2)-PI);

				if(AngleF[i][j]<=0)Angle[i][j]=AngleF[i][j]+(PI/2);
				if(AngleF[i][j]>0)Angle[i][j]=AngleF[i][j]-(PI/2);

				Angle[i][j]=Angle[i][j]*180/PI;
			}
		}

		//�̈�̉��
		free_matrix(AngleF, 0, image_x - 1, 0, image_y - 1);
		free_matrix(sGxx, 0, image_x - 1, 0, image_y - 1);
		free_matrix(sGyy, 0, image_x - 1, 0, image_y - 1);
		free_matrix(sGxy, 0, image_x - 1, 0, image_y - 1);

		return 0;
}



char propetyB[255];
char inputimage_directoryB[128];
char InputImageB[128];						//���͉摜
int read_property_Bazen(ifstream &propety_dire,int &image_x,int &image_y){
	//Bazen�̕��@��p���邽�߁Cmain.cpp�Ŏw�肵��property����摜�̏�񂾂���ǂ݂Ƃ�D�摜�T�C�Y��convolution�œǂݎ���Ă���D

	int count_propertyB=0;
	//�v���p�e�Btxt�t�@�C���̓ǂݍ���
	if (propety_dire.fail())
	{
		printf("property�e�L�X�g��ǂݎ�邱�Ƃ��ł��܂���\n");
	}
	while (propety_dire.getline(propetyB, 256 - 1)){
		if (count_propertyB == 2)sprintf(inputimage_directoryB, propetyB);
		++count_propertyB;
	}
	propety_dire.close();
	return *inputimage_directoryB;
}


int Bazen(char image_nameP2[],int &image_x,int &image_y,int paramerter[],int paramerter_count,int sd,char date[],char date_directory[]){

	printf("**************************\n");
	printf("Bazen�̕��@�ɂ��p�x����\n");
	printf("**************************\n");

	char date_directory10[128];					//�o�̓t�H���_�̃f�B���N�g��
	char *math_Bazen_atan10_s = "\\Bazen.csv";	//�o�͌��ʂ̃t�@�C�����̎w��
	char math_Bazen_atan10[128];				//�o�̓t�@�C���̃f�B���N�g��

////////////////////////////////////////////���͉摜�̓ǂݍ���////////////////////////////////////////////////////////////////////////////
	
	std::ifstream propety_dire;
	propety_dire.open(image_nameP2,ios::in);	//main.cpp�Ŏw�肵��property.txt���J��
	
	read_property_Bazen(propety_dire,image_x,image_y);	//property�ǂݍ���

	sprintf(InputImageB, inputimage_directoryB);	//property����ǂݎ�������͉摜������
	cv::Mat ImputImageMB = cv::imread(InputImageB);	//���͉摜�̓ǂݍ��݁Dopencv
	
//////////////////////////�����ݒ�̊m�F�p//////////////////////////////////////////////////////////////////////////////

	printf("InputImage=%s\n", InputImageB);
	printf("�摜�T�C�Y�FX=%d�CY=%d\n", image_x, image_y);

	//Output�f�B���N�g���̐ݒ�
	if(paramerter[0]==1 || paramerter[0]==2){
		sprintf(date_directory10, "%s%d�~%dsobel_Bazen_sd%d", date_directory, paramerter[paramerter_count],paramerter[paramerter_count], sd);
	}else{
		sprintf(date_directory10, "%s%dk_Bazen_sd%d", date_directory, paramerter[paramerter_count], sd);
	}

	//Output�t�H���_�̃f�B���N�g���쐬
	if (_mkdir(date_directory10) == 0) {
		printf("�t�H���_ %s ���쐬���܂���\n", date_directory10);
	}
	else {
		printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
	}

	//Output�t�@�C���̃f�B���N�g���ݒ�
	sprintf(math_Bazen_atan10, "%s%s", date_directory10, math_Bazen_atan10_s);

	//Nrutil��p�����������̊m��
	double **input_bmpB = matrix(0, image_x-1, 0, image_y-1);
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	double **AngleF = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gxx = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gyy = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gxy = matrix(0, image_x - 1, 0, image_y - 1);
	
	//������
	for (int i = 0; i < image_y; i++) {
		for (int j = 0; j < image_x; j++) {
			input_bmpB[j][i] = 0;
			Angle[j][i] = 0;
			AngleF[j][i] = 0;
			Gxx[j][i] = 0;
			Gyy[j][i] = 0;
			Gxy[j][i] = 0;
		}
	}

/////////////////////////�摜�̓ǂݎ��/////////////////////////////////////////////////////////////////////////
	for (int y = 0; y < image_y; ++y) {
		for (int x = 0; x < image_x; ++x) {
			//opencv�ł͉�f��R,G,B�̏��ŕ���ł���
			int R = 3 * x;
			int G = 3 * x + 1;
			int B = 3 * x + 2;
			//�O���[�X�P�[���ł̓ǂݍ���
			input_bmpB[x][y] = ((ImputImageMB.at<unsigned char>(y, R)+ImputImageMB.at<unsigned char>(y, G)+ImputImageMB.at<unsigned char>(y, B))/3);
			//input_bmp[x][y] = ImputImageM.at<unsigned char>(y, R)-Offset;
		}
	}

	//Bazen�̌v�Z�@
	for (int y = 0; y < image_y-1; ++y) {
		for (int x = 0; x < image_x-1; ++x) {
			Gxx[x][y]=pow(input_bmpB[x+1][y]-input_bmpB[x][y],2);
			Gyy[x][y]=pow(input_bmpB[x][y+1]-input_bmpB[x][y],2);
			Gxy[x][y]=(input_bmpB[x+1][y]-input_bmpB[x][y])*(input_bmpB[x][y+1]-input_bmpB[x][y]);
		}
	}

	Bazen_s(image_y,image_x,Gxx,Gyy,Gxy,AngleF,Angle);	//Bazen�̕��@�ɂ���Ċp�x�����߂�

	//���ʂ̏�������
	FILE *fp_Bazen;
	if((fp_Bazen=fopen(math_Bazen_atan10,"w"))==NULL){cout<<"���̓G���[ Bazen.csv���J���܂���";exit(1);}
	for(int j=0;j<image_y-1;++j){
			for(int i=0;i<image_x-1;++i){
			fprintf(fp_Bazen,"%lf,",Angle[i][j]);
			if(i==image_x - 2){fprintf(fp_Bazen,"\n");}
			}
	}
	fclose(fp_Bazen);

	//log�t�@�C���̏�������
	FILE *fp_date;
	char filename_log[128];
	sprintf(filename_log, "%s\\log_Bazen.txt",date_directory10);	//log�t�@�C���쐬�̃f�B���N�g���w��
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("log�t�@�C�����J���܂���");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date);						//����
	fprintf(fp_date,"�ۑ���     : %s\n",date_directory);			//�ۑ���
	fclose(fp_date);

	printf("log�t�@�C�� %s ��ۑ����܂���\n",filename_log);

	//�̈�̉��
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gxx, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gyy, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gxy, 0, image_x - 1, 0, image_y - 1);

	printf("finish : Bazen\n");

	return 0;
}
