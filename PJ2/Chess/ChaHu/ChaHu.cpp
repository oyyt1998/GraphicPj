#include "pch.h"
#include <iostream>
#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>

using namespace std;

#define BITMAP_ID 0x4D42
#define TEXTURE_INT 1

GLuint texture[TEXTURE_INT];

//读纹理图片
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;  
	BITMAPFILEHEADER bitmapFileHeader;    
	unsigned char    *bitmapImage;       
	int    imageIdx = 0;        
	unsigned char    tempRGB;  
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	fclose(filePtr);
	return bitmapImage;
}

//加载纹理的函数
void texload(int i, char *filename)
{
	BITMAPINFOHEADER bitmapInfoHeader;                                 
	unsigned char*   bitmapData;                                      

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,         //mipmap层次(通常为，表示最上层) 
		GL_RGB,    //该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,    //bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);    //bitmap数据指针  
}

//初始化纹理
void textureInit() {
	glGenTextures(TEXTURE_INT, texture);
	const char* csc = "E://FDU//COLLEGE//2019-2020.1//TuXing//PJ2//ChaHu//iron.bmp";
	char* cc;
	int length = strlen(csc);
	cc = new char[length + 1];
	strcpy(cc, csc);
	texload(0, cc);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //设置像素存储模式控制所读取的图像数据的行对齐方式.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大过滤，线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小过滤，线性过滤
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S方向重复
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T方向重复
}

//绘制象棋棋子
void Chess() {
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glPushMatrix();
	//圆球
	glTranslatef(0.0, 0.8, 0.0);
	glutSolidSphere(0.3, 40, 10);
	
	//小环
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidTorus(0.05, 0.2, 20.0, 30.0);
	
	//圆锥体
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glutSolidCone(0.4, 1.4, 20, 6);

	//大环
	glutSolidTorus(0.07, 0.4, 20.0, 20.0);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

float angle = 0.0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glRotatef(angle, 0.0, 0.0, 1.0);
	
	glScalef(1.05, 1.05, 1.05);

	Chess();

	glutSwapBuffers();

	//设置旋转动画
	angle += 0.05;

}

void reshape(int w, int h)
{
	glViewport(0.5, 0, (GLsizei)w, (GLsizei)h);
	
	//设置投影参数
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	
	//正交投影
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5*(GLfloat)h / (GLfloat)w, 1.5*(GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h, 1.5*(GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);

	//设置模型参数--几何体参数
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
}

void init(void)
{
	GLfloat AmbientLight[] = { 0.0, 0.0, 0.0, 1.0 };  //环境光
	GLfloat Light_Model_Ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //环境光参数

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  //镜面反射参数
	GLfloat mat_shininess[] = { 50.0 };               //高光指数

	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; //设定灯光的位置
	
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };   //灯位置(1,1,1), 最后1-开关
	GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 }; 
	GLfloat red_light[] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat green_light[] = { 0.0, 1.0, 0.0, 1.0 };

	glClearColor(0.08, 0.0, 0.0, 0.0);  //背景色
	glShadeModel(GL_SMOOTH);           //多变性填充模式

	//材质属性
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

	//灯光设置
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);   //环境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);   //散射光属性
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);  //镜面反射光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);  //环境光参数

	glEnable(GL_LIGHTING);   //开关:使用光
	glEnable(GL_LIGHT0);     //打开0#灯
	glEnable(GL_DEPTH_TEST); //打开深度测试
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);   
	glutInitWindowPosition(50, 50);  
	glutCreateWindow("一枚oyyt的象棋棋子");   
	textureInit();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}