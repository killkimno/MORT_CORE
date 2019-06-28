#pragma once
#include <Windows.h>
#include <map>
#include "cv.h"
#include <iostream>
#include <stdlib.h>
#include<vector>
#include <locale> 




#include <baseapi.h>
#include <allheaders.h>
#include <fstream>



#include <map>


class MainCore
{
protected:
	 
	HWND hDesktopWnd;
	HDC hwindowDC,hwindowCompatibleDC;
	HDC hDesktopDC;
	HBITMAP hbwindow;
	
	BITMAPINFOHEADER myBitmapHeader;

	std::vector<int> cutCodinateXList;	
	std::vector<int> cutCodinateYList;	
	std::vector<int> cutHeightList;	
	std::vector<int> cutWidthList;	
	std::vector<std::vector<int>> useColorSetList;		//����� ���׷�.
	
	bool isActiveWindow;				//Ȱ��ȭ �� ������� �̹��� ����
	bool isUseNHocrFlag;				//NHOcr ��� ����
	bool isUseWin10OCR;					//������ OCR ��� ����.
	bool isUseJpnFlag;					//�Ϻ�� ����ϴ°�
	bool isUseDBFlag;					//DB���Ͽ��� ������ �����´� ����
	bool isUseSpellcheckFlag;			//���縵 üũ.
	bool isStringUpper;					//��ҹ��� ���� �� ��.

	bool isAdvencedIMGOptionFlag;		//�̹��� ���� ���
	bool isErodeOptionFlag;				//������ ��Ʈ�� ���� ���
	bool isRGBOptionFlag;				//rgb�� �� ����
	bool isHSVOptionFlag;				//hsv�� �� ����
	float imgZoomSize;					//OCR ���� �̹��� Ȯ�� ������

	struct FiducialValuseStruct
	{
		int fiducialSS;
		int fiducialSV;

		int fiducialES;
		int fiducialEV;

		int fiducialR;
		int fiducialG;
		int fiducialB;
		FiducialValuseStruct()
		{
		 fiducialSS = 0;
		 fiducialSV = 0;

		 fiducialES = 0;
		 fiducialEV = 0;

		 fiducialR = 0;
		 fiducialG = 0;
		 fiducialB = 0;
		}
	};
	std::vector<FiducialValuseStruct> fiducialValuseList;	
		struct TranslationsDB
	{
		std::wstring translationText;					//������
		std::wstring originalText;						//���� �Ǵ� ocr�� ����� �ؽ�Ʈ
		std::wstring altText;							//�Ϻ��� ���� �ؽ�Ʈ(������ �ؽ�Ʈ)
		std::vector<std::wstring> originalTextList;		//�ܾ� ������ ���� ������ �ܾ�
		int textLength;
		int tokenValue;									//��ū ��
		std::vector<int> wordValueList;
		int value;
		int errorValue;
		int errorValue2;
		int index;										//�ε���
		TranslationsDB()
		{
			tokenValue = 0;
			value = 0;
			errorValue = 0;	
			errorValue2 = 0;
			textLength = 0;
			index = 0;
		}

	};


	struct DicDB
	{
		int tokenSize;
		std::wstring originalText;
		std::vector<std::wstring> originalTextList;
		std::wstring resultText;
		DicDB()
		{
			originalText=L"";
			tokenSize=0;
		}
	};

	
	struct CharacterTable
	{
		int characteValue[52][3];		//0 = �� 1 = ���� ���� 2 = ���� �߻�

		CharacterTable()
		{
			for(int i = 0; i!=52; i++)
			{
				for(int y= 0; y!=3; y++)
				{
					characteValue[i][y] = 0;
				}
				
			}
		}
	};

	CharacterTable myCharacterTable;

	std::vector<TranslationsDB> myTranslationVector;
	std::multimap<int,TranslationsDB> myTranslationMap;
	std::multimap<std::wstring, DicDB> myDicMap;

	struct DebugStruct
	{
		long clientCoordinateX;
		long clientCoordinateY;

		std::wstring debug;

	};


private:
	void initScreenCapture();
	int lastDBIndex;

public:
	MainCore();
	 ~MainCore();
	 int testInt;
	 DebugStruct debugStruct;
	 void init();
	 void setDefaultOption();
	 void SetIsStringUpper(bool isUpper);	//��ҹ��� ���� ���� ����.
	 void SetIsActiveWindow(bool isActiveWindow);
	 void SetIsUseNHocr(bool isUseNHocr);
	 void setIsUseJpnFlag(bool newJpnFlag);
	 void setFiducialValue(int newFiducialR[], int newFiducialG[], int newFiducialB[], int newFiducialSS[], int newFiducialES[], int newFiducialSV[], int newFiducialEV[], int newSize);
	 void setUseDB(bool newIsUseDBFlag ,  char *dbFileName);
	 bool getUseDBFlag();
	 bool GetIsUseNHocr();
	 void setAdvencedImgOption(bool newIsUseRGBFlag, bool newIsUseHSVFlag, bool newIsUseErodeFlag, float newImgZoomSize);
	 void openSettingFile(char *dbFileName);
	 bool sequentialSearchDB(TranslationsDB *newDB);
	 bool searchDB(TranslationsDB *ocrDB , TranslationsDB *resultDB, int adjustRange, int mapTokenIndex, int *nowDistance, int *nowTextDistance);
	 bool searchJpnDB(TranslationsDB *ocrDB , TranslationsDB *resultDB, int adjustRange, int mapTokenIndex, int *nowDistance, int *nowTextDistance);
	 std::wstring getTranslation(std::wstring text);
	 int compareWordList(std::vector<int> ocrWordValueList, std::vector<int> dbWordValueList, int ocrCount, int dbCount);
	 int compareOriginalText(std::wstring inputText, std::wstring resultText);
	 bool equalWord(int ocrValue, int dbValue, int ocrCount);
	 void analysisText(std::wstring text, struct TranslationsDB * newDB);
	 void setCutPoint(int newX[], int newY[], int newHeight[], int newWidth[], int newSize);
	 void ClearOcrColorSet();
	 void AddOcrColorSet(int colorList[], int size);
	 void getScreen(cv::Mat *newImg, int captureIndex);
	 void adjustImg(cv::Mat* img, int captureIndex);


	 bool getUseCheckSpellingFlag();
	 void setUseCheckSpellingFlag(bool newUseCheckspellFlag, char *newDicFileText);

	 std::wstring checkSpelling(std::wstring text , bool* isReplaceFlag);
	 std::wstring GetEnglishSpellingCheck(std::wstring text, bool* isReplaceFlag);
	 std::wstring GetJpnSpellingCheck(std::wstring text, bool* isReplaceFlag);
	 void openDicFile(char *newDicFileText);
	 void readJPNDicFile(char *dicFileText);
	 void readENGDicFile(char *dicFileText);

	 void setErode(bool newFlag);
	 int isUTF8(const char *data, size_t size);
	 static int utf;

};