#pragma once
#include <Windows.h>
#include <map>
#include "opencv.hpp"
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

	std::vector<int> exceptCodinateXList;
	std::vector<int> exceptCodinateYList;
	std::vector<int> exceptHeightList;
	std::vector<int> exceptWidthList;

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
	bool isUsePartialDB;				//�κ���ġ �˻� �����.
	bool isUseSpellcheckFlag;			//���縵 üũ.
	bool isUseMatchWordDic;		//���縵 üũ.
	int reCehckSpllingCount;	//�������� �߰� �˻� Ƚ��
	bool isStringUpper;					//��ҹ��� ���� �� ��.

	bool isAdvencedIMGOptionFlag;		//�̹��� ���� ���
	bool isErodeOptionFlag;				//������ ��Ʈ�� ���� ���
	bool isRGBOptionFlag;				//rgb�� �� ����
	bool isHSVOptionFlag;				//hsv�� �� ����
	bool isThresHold;					//�Ӱ�ġ ���
	int thresholdValue;
	float imgZoomSize;					//OCR ���� �̹��� Ȯ�� ������

	bool isRemoveSpace = false;					//���� ����.
	bool isShowOCRIndex = false;				//OCR �ε��� ǥ��

	struct DebugMode					//����� ���.
	{
	public :
		bool isActive;
		bool isShowReplace;
		bool isSaveCapture;
		bool isSaveCaptureResult;

		DebugMode()
		{
			isActive = false;
			isShowReplace = false;
			isSaveCapture = false;
			isSaveCaptureResult = false;
		}
	};

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
	 int monitor_count;


	 DebugMode debugMode;
	 DebugStruct debugStruct;
	 void init();
	 void setDefaultOption();
	 void SetIsStringUpper(bool isUpper);	//��ҹ��� ���� ���� ����.
	 void SetIsActiveWindow(bool isActiveWindow);
	 void SetIsUseNHocr(bool isUseNHocr);
	 void setIsUseJpnFlag(bool newJpnFlag);
	 bool GetIsUseJpnFlag();
	 void setFiducialValue(int newFiducialR[], int newFiducialG[], int newFiducialB[], int newFiducialSS[], int newFiducialES[], int newFiducialSV[], int newFiducialEV[], int newSize);
	 void setUseDB(bool newIsUseDBFlag ,  bool _IsUsePartialDB,char *dbFileName);
	 bool getUseDBFlag();
	 bool GetIsUsePartialDB();
	 bool GetIsUseNHocr();
	 void setAdvencedImgOption(bool newIsUseRGBFlag, bool newIsUseHSVFlag, bool newIsUseErodeFlag, float newImgZoomSize, bool isUseThreshold, int thresholdValue);
	 void openSettingFile(char *dbFileName);
	 bool sequentialSearchDB(TranslationsDB *newDB);
	 bool searchDB(TranslationsDB *ocrDB , TranslationsDB *resultDB, int adjustRange, int mapTokenIndex, int *nowDistance, int *nowTextDistance);
	 bool searchJpnDB(TranslationsDB *ocrDB , TranslationsDB *resultDB, int adjustRange, int mapTokenIndex, int *nowDistance, int *nowTextDistance, int minCardinate);
	 std::wstring getTranslation(std::wstring text, bool& isFound);
	 int compareWordList(std::vector<int> ocrWordValueList, std::vector<int> dbWordValueList, int ocrCount, int dbCount);
	 int compareOriginalText(std::wstring inputText, std::wstring resultText);
	 bool equalWord(int ocrValue, int dbValue, int ocrCount);
	 void analysisText(std::wstring text, struct TranslationsDB * newDB);
	 void setCutPoint(int newX[], int newY[], int newHeight[], int newWidth[], int newSize);
	 void SetExceptPoint(int newX[], int newY[], int newHeight[], int newWidth[], int newSize);	//���� ����.
	 void ClearOcrColorSet();
	 void AddOcrColorSet(int colorList[], int size);
	 void getScreen(cv::Mat *newImg, int captureIndex);
	 void getScreen(cv::Mat* newImg, int captureIndex , int *locationX, int *locationY);
	 void getScreen2(cv::Mat* newImg, int captureIndex, uint8_t* data, int _width, int _height , int positionX, int positionY);
	 void adjustImg(cv::Mat* img, int captureIndex);
	 void RemoveAreaImg(cv::Mat* newImg, int captureIndex);


	 bool getUseCheckSpellingFlag();
	 void setUseCheckSpellingFlag(bool newUseCheckspellFlag, bool _isMatchingWord, char *newDicFileText);
	 void SetReCheckSpellingCount(int reCheckCount);

	 std::wstring checkSpelling(std::wstring text , bool* isReplaceFlag, std::wstring toekn);
	 std::wstring GetMatchingSpellingCheck(std::wstring text, bool* isReplaceFlag);
	 std::wstring GetLetterSpellingCheck(std::wstring text, bool* isReplaceFlag);
	 void openDicFile(char *newDicFileText);
	 void ReadUnMatchingDicFile(char *dicFileText);
	 void ReadMatchingDicFile(char *dicFileText);

	 void setErode(bool newFlag);
	 void SetRemoveSpace(bool isRemove);
	 void SetShowOCRIndex(bool isShow);
	 bool GetIsRemoveSpace();
	 bool GetIsShowOCRIndex();

	 void ReplaceAll(std::wstring& strSrc, const std::wstring& strFind, const std::wstring& strDest);
	 std::vector<std::wstring> StringSplite(std::wstring text, std::wstring toekn, int minSize);
	

};