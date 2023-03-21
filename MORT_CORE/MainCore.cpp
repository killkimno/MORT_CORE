#include "MainCore.h"

#include "opencv.hpp"
#include "highgui.hpp"
#include "ml.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <baseapi.h>
#include <allheaders.h>
#include <fstream>
#include <iostream>

#include <string> 

#include "StringTokenizer.h"
#include <map>

const int PER_GET_TEXT = 2000;

void MainCore::ReplaceAll(std::wstring& strSrc, const std::wstring& strFind, const std::wstring& strDest)
{
	size_t j;
	while ((j = strSrc.find(strFind)) != (std::wstring::npos))
		strSrc.replace(j, strFind.length(), strDest);
}


std::wstring stringToWstring(std::string originalString)
{
	std::wstring wGetstring;
	wchar_t* wChar;
	int len = MultiByteToWideChar(CP_ACP, 0, originalString.c_str(), -1, NULL, 0);

	wChar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, originalString.c_str(), -1, wChar, len);
	wGetstring = wChar;

	delete[]wChar;

	return wGetstring;

}

std::wstring utfStringToWstring(std::string originalString)
{
	std::wstring wGetstring;
	wchar_t* wChar;
	int len = MultiByteToWideChar(CP_UTF8, 0, originalString.c_str(), -1, NULL, 0);
	if (len != 0)
	{
		wChar = new wchar_t[len + 1];
		MultiByteToWideChar(CP_UTF8, 0, originalString.c_str(), -1, wChar, len);
	}

	wGetstring = wChar;

	delete[]wChar;

	return wGetstring;

}


std::string WstringToString(std::wstring originalString)
{
	std::string message_a;
	message_a.assign(originalString.begin(), originalString.end());

	return message_a;
}





std::vector<std::wstring> MainCore::StringSplite(std::wstring text, std::wstring token, int minSize = 0)
{
	std::wcout << std::endl << L" start string spilite ?" << std::endl;
	std::vector<std::wstring> vector;


	StringTokenizer tokens(text, token);
	std::wcout << std::endl << L" make token ?" << std::endl;
	int size = tokens.countTokens();
	std::wcout << std::endl << L" token size ?" << size << std::endl;
	int nowTokenWordSize = 0;
	std::wstring splite = L"";

	bool isPush = false;

	if (minSize == -1)
	{
		isPush = true;
	}

	for (int x = 1; tokens.hasMoreTokens(); x++)
	{
		std::wcout << std::endl << L"token ?" << x << std::endl;

		std::wstring tx = tokens.nextToken();// +L"\r\n";
		//std::wcout << std::endl << tx.size() << std::endl;
		if (minSize == -1 || tx.size() >= minSize)
		{

			if (!isPush)
			{
				vector.push_back(splite);
				//std::wcout << L"##" << splite << std::endl;
			}
			isPush = true;

			vector.push_back(tx);

			splite = L"";

			//std::wcout << tx << std::endl;
		}
		else
		{
			splite = splite + tx;

			isPush = false;
		}
	}

	if (!isPush)
	{
		vector.push_back(splite);
		//std::wcout << L"##" << splite << std::endl;
	}


	std::wcout << std::endl << L" end string spilite ?" << std::endl;

	return vector;

}



MainCore::MainCore()
{
	lastDBIndex = 0;
}



void MainCore::init()
{
	initScreenCapture();
	setDefaultOption();

}
void MainCore::setErode(bool newFlag)
{
	isErodeOptionFlag = newFlag;
}

void MainCore::SetRemoveSpace(bool isRemove)
{
	isRemoveSpace = isRemove;
}

void MainCore::SetShowOCRIndex(bool isShow)
{
	isShowOCRIndex = isShow;
}

void MainCore::setDefaultOption()
{
	isActiveWindow = false;
	isUseNHocrFlag = false;
	isAdvencedIMGOptionFlag = false;		//이미지 보정 기능
	isErodeOptionFlag = false;				//볼드형 폰트를 위한 기능
	isRGBOptionFlag = false;				//rgb로 색 구분
	isHSVOptionFlag = false;				//hsv로 색 구분
	isUseDBFlag = false;
	isUseSpellcheckFlag = true;
	FiducialValuseStruct newFiducailStruct = FiducialValuseStruct();
	fiducialValuseList.push_back(newFiducailStruct);
	/*
	cutCodinateX = 5;
	cutCodinateY = 5;

	cutHeight = 5;
	cutWidth = 5;
	*/
}

void MainCore::SetIsStringUpper(bool isUpper)
{
	this->isStringUpper = isUpper;
}

void MainCore::SetIsActiveWindow(bool isActiveWindow)
{
	this->isActiveWindow = isActiveWindow;
}

void MainCore::SetIsUseNHocr(bool isUseNHocr)
{
	isUseNHocrFlag = isUseNHocr;
	if (isUseNHocrFlag)
		isUseJpnFlag = true;
}

bool MainCore::GetIsUseNHocr()
{
	return isUseNHocrFlag;
}

void MainCore::setIsUseJpnFlag(bool newJpnFlag)
{
	isUseJpnFlag = newJpnFlag;
}

bool MainCore::GetIsUseJpnFlag()
{
	return isUseJpnFlag;
}

void MainCore::setFiducialValue(int newFiducialR[], int newFiducialG[], int newFiducialB[], int newFiducialSS[], int newFiducialES[], int newFiducialSV[], int newFiducialEV[], int newSize)
{
	fiducialValuseList.clear();
	for (int i = 0; i < newSize; i++)
	{
		FiducialValuseStruct newFiducailStruct = FiducialValuseStruct();
		newFiducailStruct.fiducialR = newFiducialR[i];
		newFiducailStruct.fiducialG = newFiducialG[i];
		newFiducailStruct.fiducialB = newFiducialB[i];

		newFiducailStruct.fiducialSS = newFiducialSS[i];
		newFiducailStruct.fiducialSV = newFiducialSV[i];
		newFiducailStruct.fiducialES = newFiducialES[i];
		newFiducailStruct.fiducialEV = newFiducialEV[i];
		fiducialValuseList.push_back(newFiducailStruct);
	}
}


void MainCore::setUseDB(bool newIsUseDBFlag, bool _IsUsePartialDB, char* dbFileName)
{
	isUseDBFlag = newIsUseDBFlag;
	isUsePartialDB = _IsUsePartialDB;

	if (isUseDBFlag == true)
	{
		openSettingFile(dbFileName);
	}

}


bool MainCore::GetIsUsePartialDB()
{
	return isUsePartialDB;
}

bool MainCore::getUseDBFlag()
{
	return isUseDBFlag;
}

bool MainCore::GetIsRemoveSpace()
{
	return isRemoveSpace;
}

bool MainCore::GetIsShowOCRIndex()
{
	return isShowOCRIndex;
}

bool MainCore::getUseCheckSpellingFlag()
{
	return isUseSpellcheckFlag;
}
void MainCore::setUseCheckSpellingFlag(bool newUseCheckSpellingFlag, bool _isMatchingWord, char* newFileName)
{
	isUseMatchWordDic = _isMatchingWord;
	isUseSpellcheckFlag = newUseCheckSpellingFlag;
	openDicFile(newFileName);
}

void MainCore::SetReCheckSpellingCount(int reCheckCount)
{
	reCehckSpllingCount = reCheckCount;
	std::cout << std::endl << "reCehckSpllingCount" << reCehckSpllingCount << std::endl;
}

void MainCore::setAdvencedImgOption(bool newIsUseRGBFlag, bool newIsUseHSVFlag, bool newIsUseErodeFlag, float newImgZoomSize, bool isUseThreshold, int thresholdValue)
{
	isRGBOptionFlag = newIsUseRGBFlag;
	isHSVOptionFlag = newIsUseHSVFlag;
	isErodeOptionFlag = newIsUseErodeFlag;
	imgZoomSize = newImgZoomSize;

	isThresHold = isUseThreshold;
	this->thresholdValue = thresholdValue;

	if (isRGBOptionFlag == false && isHSVOptionFlag == false && isUseThreshold == false)
	{
		isAdvencedIMGOptionFlag = false;
	}
	else
	{
		isAdvencedIMGOptionFlag = true;
	}
}


bool MainCore::equalWord(int ocrValue, int dbValue, int ocrCount)
{
	bool isEqualFlag = false;
	int range = (dbValue + ocrValue) / 5;

	if (ocrValue - dbValue >= -range && ocrValue - dbValue <= range)

		isEqualFlag = true;
	else
		isEqualFlag = false;

	return isEqualFlag;
}

int MainCore::compareOriginalText(std::wstring inputText, std::wstring resultText)
{
	ReplaceAll(inputText, L" ", L"");
	ReplaceAll(resultText, L" ", L"");
	ReplaceAll(inputText, L".", L"");
	ReplaceAll(resultText, L".", L"");

	int ocrCount = inputText.size();
	int dbCount = resultText.size();



	if (ocrCount == 0 || dbCount == 0)
	{
		return 100;
	}

	int** d = new int* [ocrCount + 1];
	for (int i = 0; i < ocrCount + 1; ++i)
		d[i] = new int[dbCount + 1];

	for (int i = 0; i <= ocrCount; i++)
		d[i][0] = i;
	for (int j = 0; j <= dbCount; j++)
		d[0][j] = j;
	for (int i = 1; i <= ocrCount; i++)
	{
		for (int j = 1; j <= dbCount; j++)
		{

			if (inputText[i - 1] == resultText[j - 1])
			{

				d[i][j] = d[i - 1][j - 1];
			}
			else
			{
				d[i][j] = std::min(d[i - 1][j] + 1, std::min(d[i][j - 1] + 1, d[i - 1][j - 1] + 1));
			}
		}
	}


	int result = d[ocrCount - 1][dbCount - 1];

	for (int i = 0; i < ocrCount + 1; i++)
	{
		delete[] d[i];
	}
	delete[] d;

	return result;
}
//문장의 단어값 비교
int MainCore::compareWordList(std::vector<int> ocrWordValueList, std::vector<int> dbWordValueList, int ocrCount, int dbCount)
{
	if (ocrCount == 0 || dbCount == 0)
	{
		return 999;		//토큰 없으면 걍 떄려침
	}

	// Allocate distance matrix
	int** d = new int* [ocrCount + 1];
	for (int i = 0; i < ocrCount + 1; ++i)
		d[i] = new int[dbCount + 1];

	// Get character comparer
	/*
	CharComparer isEqual = (ignoreCase) ?
		(CharComparer)CharCompareIgnoreCase : CharCompare;
		*/

		// Compute distance
	for (int i = 0; i <= ocrCount; i++)
		d[i][0] = i;
	for (int j = 0; j <= dbCount; j++)
		d[0][j] = j;
	for (int i = 1; i <= ocrCount; i++)
	{
		for (int j = 1; j <= dbCount; j++)
		{

			if (equalWord(ocrWordValueList[i - 1], dbWordValueList[j - 1], ocrCount))
			{
				// No change required
				d[i][j] = d[i - 1][j - 1];
			}
			else
			{
				d[i][j] = std::min(d[i - 1][j] + 1, std::min(d[i][j - 1] + 1, d[i - 1][j - 1] + 1));
			}
		}
	}

	// Return final value
	//return d[ocrCount -1, dbCount-1 ];


	int result = d[ocrCount - 1][dbCount - 1];

	for (int i = 0; i < ocrCount + 1; i++)
	{
		delete[] d[i];
	}
	delete[] d;



	return result;
}
bool MainCore::sequentialSearchDB(TranslationsDB* newDB)
{
	int findIndex = lastDBIndex;
	bool isFindDBFlag = false;

	for (int i = findIndex; i < lastDBIndex + 3 && i < myTranslationVector.size(); i++)
	{
		int dbToken = myTranslationVector[i].tokenValue;
		int dbValue = myTranslationVector[i].value;
		int dbTextSize = myTranslationVector[i].originalText.size();
		std::wstring dbText = myTranslationVector[i].originalText;
		if (dbValue - 200 <= newDB->value && newDB->value <= dbValue + 200)
		{
			if (dbToken - 1 <= newDB->tokenValue && newDB->tokenValue <= dbToken + 1)
			{
				int distance = compareOriginalText(newDB->originalText, dbText);
				if (distance <= dbTextSize / 11)
				{
					lastDBIndex = i;
					isFindDBFlag = true;
					break;
				}
			}

		}
	}



	return isFindDBFlag;
}

bool MainCore::searchDB(TranslationsDB* newDB, TranslationsDB* resultDB, int adjustRange, int mapTokenIndex, int* nowDistance, int* nowTextDistance)
{
	bool isFindTranslationFlag = false;
	std::pair <std::multimap<int, TranslationsDB>::iterator, std::multimap<int, TranslationsDB>::iterator> mapPD;
	mapPD = myTranslationMap.equal_range(mapTokenIndex);
	int nowIndex = 0;
	bool isCandidateFlag = false;		//세부 검사를 할 때 이미 후보자가 있을 시 안 함

	for (std::multimap<int, TranslationsDB>::iterator it = mapPD.first; it != mapPD.second; ++it)
	{
		if (it->second.value >= newDB->value - adjustRange
			&& it->second.value <= newDB->value + adjustRange)
		{
			int newDistance = compareWordList(newDB->wordValueList, it->second.wordValueList, newDB->tokenValue, it->second.tokenValue);
			if (newDistance == 0 && it->second.value == newDB->value)
			{
				int originalTextdistance = compareOriginalText(newDB->originalText, it->second.originalText);
				if (originalTextdistance <= it->second.originalText.size() / 11)
				{
					*resultDB = it->second;
					*nowDistance = newDistance;
					isFindTranslationFlag = true;
					break;
				}

			}

			if ((newDistance == *nowDistance || newDistance - 1 == *nowDistance || newDistance + 1 == *nowDistance) && isCandidateFlag == false)
			{
				int newExtraDistance = compareOriginalText(newDB->originalText, it->second.originalText);
				if (newExtraDistance < *nowTextDistance)
				{
					*resultDB = it->second;
					*nowDistance = newDistance;
					*nowTextDistance = newExtraDistance;
					if (*nowTextDistance <= resultDB->originalText.size() / 11)
					{
						isFindTranslationFlag = true;
						break;
					}
					else if (*nowTextDistance <= resultDB->originalText.size() / 7)
					{
						isCandidateFlag = true;
					}
					else
					{
						isCandidateFlag = false;
					}
				}

			}

			else if (newDistance < *nowDistance)
			{

				if (isCandidateFlag == true)
				{
					int newTextDistance = compareOriginalText(it->second.originalText, resultDB->originalText);
					if (*nowTextDistance * 1.2f > newTextDistance)
					{
						*resultDB = it->second;
						*nowDistance = newDistance;
						*nowTextDistance = newTextDistance;
						isCandidateFlag = true;

						if (newTextDistance <= it->second.originalText.size() / 11)
						{
							isFindTranslationFlag = true;
							break;
						}
					}
				}
				else
				{
					if (newDistance <= it->second.tokenValue * 4 / 10)
					{
						int newExtraDistance = compareOriginalText(newDB->originalText, it->second.originalText);

						if (newExtraDistance <= it->second.originalText.size() * 38 / 100)
						{
							*resultDB = it->second;
							*nowDistance = newDistance;
							*nowTextDistance = newExtraDistance;
							int nowResultDBTextSize = it->second.originalText.size();
							if (newExtraDistance <= nowResultDBTextSize / 11)
							{
								isFindTranslationFlag = true;
								break;
							}
							else if (newExtraDistance <= nowResultDBTextSize / 7)
							{
								isCandidateFlag = true;
							}
							else
							{
								isCandidateFlag = false;
							}
						}


					}


				}


			}

		}
		nowIndex++;

	}

	return isFindTranslationFlag;
}


bool MainCore::searchJpnDB(TranslationsDB* newDB, TranslationsDB* resultDB, int adjustRange, int mapTokenIndex, int* nowDistance, int* nowTextDistance, int minCardinateDistance)
{
	bool isFindTranslationFlag = false;
	std::pair <std::multimap<int, TranslationsDB>::iterator, std::multimap<int, TranslationsDB>::iterator> mapPD;
	mapPD = myTranslationMap.equal_range(mapTokenIndex);
	int nowIndex = 0;
	bool isCandidateFlag = false;		//세부 검사를 할 때 이미 후보자가 있을 시 안 함

	for (std::multimap<int, TranslationsDB>::iterator it = mapPD.first; it != mapPD.second; ++it)
	{
		if (it->second.value >= newDB->value - adjustRange
			&& it->second.value <= newDB->value + adjustRange)
		{
			int newDistance = abs((int)((it->second.textLength - newDB->textLength) / 5));
			if (newDistance == 0 && it->second.value == newDB->value && it->second.altText == newDB->altText)
			{
				std::wcout << "Find? : " << it->second.altText << std::endl;
				*resultDB = it->second;
				*nowDistance = newDistance;
				isFindTranslationFlag = true;
				break;
			}
			
			int rangeValue = *nowDistance == 100 ? newDistance :( newDistance - *nowDistance );

			//std::wcout <<"adjust " << adjustRange << " value " << it->second.value << " ocr value " << newDB->value <<  "range : " << rangeValue << " dis " << *nowDistance << "text dis : " << *nowTextDistance << std::endl;

			if ((-3 <= rangeValue && rangeValue <= 3) && isCandidateFlag == false)
			{

				int newExtraDistance = compareOriginalText(newDB->altText, it->second.altText);

				//std::wcout << "Search alt :" << newDB->altText << " // " << it->second.altText << "// length : " << resultDB->textLength << "new dis :" << newExtraDistance << std::endl;
				if (newExtraDistance < *nowTextDistance)
				{
					std::wcout << "change alt alt :" << std::endl;
					*resultDB = it->second;
					*nowDistance = newDistance;
					*nowTextDistance = newExtraDistance;
					if ((resultDB->textLength > 5 && abs(*nowTextDistance) <= 2) || *nowTextDistance <= resultDB->textLength / 11)
					{
						*resultDB = it->second;
						*nowDistance = newDistance;
						isFindTranslationFlag = true;
						break;
					}
					else if (minCardinateDistance == -1)
					{
						if (*nowTextDistance <= resultDB->textLength / 4)
						{
							isCandidateFlag = true;
						}
						else
						{
							isCandidateFlag = false;
						}
					}
					else if (newExtraDistance <= minCardinateDistance)
					{
						*resultDB = it->second;
						*nowDistance = newDistance;
						isFindTranslationFlag = true;
						break;
					}
				
				}
			}

			else if (newDistance < *nowDistance)
			{
				if (isCandidateFlag == true)
				{
					int newTextDistance = compareOriginalText(it->second.altText, resultDB->altText);
					if (*nowTextDistance * 1.2f > newTextDistance)
					{
						*resultDB = it->second;
						*nowDistance = newDistance;
						*nowTextDistance = newTextDistance;
						isCandidateFlag = true;

						if (newTextDistance <= it->second.originalText.size() / 11)
						{
							isFindTranslationFlag = true;
							break;
						}
					}
				}
				else
				{
					*nowTextDistance = compareOriginalText(newDB->altText, it->second.altText);

					if (*nowTextDistance <= it->second.textLength * 60 / 100)
					{
						*resultDB = it->second;
						*nowDistance = newDistance;
						int nowResultDBTextSize = resultDB->textLength;
						if (*nowTextDistance <= nowResultDBTextSize / 11)
						{
							*resultDB = it->second;
							*nowDistance = newDistance;
							isFindTranslationFlag = true;
							break;
						}
						else if (*nowTextDistance <= nowResultDBTextSize / 5)
						{
							isCandidateFlag = true;
						}
						else
						{
							isCandidateFlag = false;
						}
					}

				}

			}

		}
		nowIndex++;
	}

	return isFindTranslationFlag;
}




std::wstring MainCore::getTranslation(std::wstring text, bool& isFound)
{
	isFound = true;

	int nowDistance = 100;
	int nowTextDistance = 100;

	std::pair <std::multimap<int, TranslationsDB>::iterator, std::multimap<int, TranslationsDB>::iterator> mapPD;
	TranslationsDB newDB;
	newDB = TranslationsDB();
	TranslationsDB resultDB;
	resultDB = TranslationsDB();



	analysisText(text, &newDB);


	bool isFindTranslationFlag = false;
	int adjustRange = 0;
	int mapTokenIndex = newDB.tokenValue;
	int adustTokenRange = 0;

	//순차 검색을 미리 해봄 (2개)
	if (sequentialSearchDB(&newDB) == true)
	{
		return myTranslationVector[lastDBIndex].translationText;
	}


	if (isUseJpnFlag == false)
	{
		if (mapTokenIndex <= 7)
		{
			adustTokenRange = 2;
		}
		else if (mapTokenIndex <= 14)
		{
			adustTokenRange = (float)(mapTokenIndex * 0.28f);
		}
		else
		{
			adustTokenRange = (float)(mapTokenIndex * 0.21f);
		}
		int startToken = mapTokenIndex - adustTokenRange;
		int endToken = mapTokenIndex + adustTokenRange;

		adjustRange = 0;

		isFindTranslationFlag = searchDB(&newDB, &resultDB, adjustRange, mapTokenIndex, &nowDistance, &nowTextDistance);
		if (isFindTranslationFlag == true)
		{
			lastDBIndex = resultDB.index;
			return resultDB.translationText;

		}


		adjustRange = (newDB.tokenValue * 100);
		isFindTranslationFlag = searchDB(&newDB, &resultDB, adjustRange, mapTokenIndex, &nowDistance, &nowTextDistance);
		if (isFindTranslationFlag == true)
		{
			lastDBIndex = resultDB.index;
			return resultDB.translationText;;

		}


		for (; startToken <= endToken; startToken++)
		{
			if (startToken != mapTokenIndex && startToken > 0)
			{
				isFindTranslationFlag = searchDB(&newDB, &resultDB, adjustRange, startToken, &nowDistance, &nowTextDistance);
				if (isFindTranslationFlag == true)
				{
					lastDBIndex = resultDB.index;
					return resultDB.translationText;
				}
			}
		}




		if (nowDistance <= (resultDB.tokenValue * 4 / 10) && isFindTranslationFlag == false)
		{
			lastDBIndex = resultDB.index;
			return resultDB.translationText;;
		}
	}
	else if (isUseJpnFlag == true)
	{
		if (mapTokenIndex <= 7)
		{
			adustTokenRange = 5;
		}
		else
		{
			adustTokenRange = (float)(mapTokenIndex * 0.6f);
		}

		int startToken = mapTokenIndex - adustTokenRange;
		int endToken = mapTokenIndex + adustTokenRange;
		adjustRange = 0;

		std::wcout << std::endl <<  "Start Jpn  : Token " << adjustRange << " map " << mapTokenIndex <<  "/ token 1 : " << startToken << " / end : " << endToken << std::endl;

		isFindTranslationFlag = searchJpnDB(&newDB, &resultDB, adjustRange, mapTokenIndex, &nowDistance, &nowTextDistance, -1);
		if (isFindTranslationFlag == true)
		{
			std::wcout << "Found  step1" << std::endl;
			lastDBIndex = resultDB.index;
			return resultDB.translationText;
		}
		adjustRange = (newDB.originalText.length() * 700);
		isFindTranslationFlag = searchJpnDB(&newDB, &resultDB, adjustRange, mapTokenIndex, &nowDistance, &nowTextDistance, -1);

		if (isFindTranslationFlag == true)
		{
			std::wcout << "Found  step2" << std::endl;
			lastDBIndex = resultDB.index;
			return resultDB.translationText;;
		}


		for (; startToken <= endToken; startToken++)
		{
			if (startToken != mapTokenIndex && startToken > 0)
			{
				isFindTranslationFlag = searchJpnDB(&newDB, &resultDB, adjustRange, startToken, &nowDistance, &nowTextDistance, 4);
				std::wcout << "step3 cardi : token " << startToken <<" distance : "<<nowTextDistance << " text : " << resultDB.originalText << std::endl;
		
				if (isFindTranslationFlag == true)
				{
					std::wcout << "step3 Found!!!!! :" << resultDB.originalText << std::endl;
					lastDBIndex = resultDB.index;
					return resultDB.translationText;
				}
			}
		}
		if (nowTextDistance <= (resultDB.textLength * 4 / 10) && isFindTranslationFlag == false)
		{
			std::wcout << "step4 .... :" << resultDB.originalText << std::endl;
			lastDBIndex = resultDB.index;
			return resultDB.translationText;;
		}
	}


	isFound = false;
	return L"not thing";



	//std::multimap<int,TranslationsDB>::iterator mapPD = myTranslationMap.find(newDB->tokenValue);
}


//문장을 DB 형식으로 분석, newDB에 저장
void MainCore::analysisText(std::wstring text, struct TranslationsDB* newDB)
{


	std::wstring tokenString;
	std::wstring analysisText;

	//strcat(analysisText, text.c_str());
	int nowWordValue = 0;
	int nowIndex = 0;
	int nowValue = 0;
	int nowTokenValue = 0;
	int errorValue = 0;
	int errorValue2 = 0;

	int textSize = 0;
	int nowWordIndex = 0;


	ReplaceAll(text, L"\r\n", L" ");

	if (isUseJpnFlag == false)
	{
		if (isStringUpper)
		{
			std::transform(text.begin(), text.end(), text.begin(), ::toupper);
		}

		StringTokenizer tokens(text, L" ");
		int size = tokens.countTokens();
		int nowTokenWordSize = 0;
		for (int x = 1; tokens.hasMoreTokens(); x++)
		{
			tokenString = tokens.nextToken();
			analysisText = tokenString;
			//strcpy(analysisText, tokenString.c_str());
			newDB->originalTextList.push_back(tokenString.c_str());
			for (int i = 0; i < tokenString.size(); i++)
			{


				if (analysisText[i] >= 65 && analysisText[i] <= 90)
				{
					int index = (int)analysisText[i] - 65;
					nowValue = nowValue + (int)myCharacterTable.characteValue[index][0];
					errorValue = errorValue + (int)myCharacterTable.characteValue[index][1];
					errorValue2 = errorValue2 + (int)myCharacterTable.characteValue[index][2];
					nowWordValue = nowWordValue + (int)myCharacterTable.characteValue[index][0];
					nowTokenWordSize++;

				}
				else if (analysisText[i] >= 97 && analysisText[i] <= 122)
				{

					int index = (int)analysisText[i] - 71;
					nowValue = nowValue + (int)myCharacterTable.characteValue[index][0];
					errorValue = errorValue + (int)myCharacterTable.characteValue[index][1];
					errorValue2 = errorValue2 + (int)myCharacterTable.characteValue[index][2];
					nowWordValue = nowWordValue + (int)myCharacterTable.characteValue[index][0];
					nowTokenWordSize++;
				}


			}
			if (nowTokenWordSize >= 3)
			{
				nowTokenValue++;

				newDB->wordValueList.push_back(nowWordValue);

			}
			nowWordValue = 0;
			nowTokenWordSize = 0;


		}


		newDB->errorValue = errorValue;
		newDB->errorValue2 = errorValue2;
		newDB->originalText = text;
		newDB->tokenValue = nowTokenValue;
		newDB->value = nowValue;
	}
	else if (isUseJpnFlag == true)
	{
		int tokenSize = 0;
		for (int i = 0; i < text.length(); i++)
		{
			if (text[i] >= 0x4E00 && text[i] <= 0x9FFF)
			{
				tokenSize++;
			}
		}

		if (tokenSize == 0)
		{
			tokenSize = 1;
		}
		for (int i = 0; i < text.length(); i++)
		{
			if (text[i] >= 0x3041 && text[i] <= 0x309F)
			{
				nowValue = nowValue + (int)text[i] - 12293;
				nowWordValue = nowWordValue + (int)text[i] - 12293;
				newDB->altText = newDB->altText + text[i];
			}
			else if (text[i] >= 0x30A1 && text[i] <= 0x30FF)
			{
				nowValue = nowValue + (int)text[i] - 12343;
				nowWordValue = nowWordValue + (int)text[i] - 12343;
				newDB->altText = newDB->altText + text[i];
			}
			else if (text[i] >= 0x4E00 && text[i] <= 0x9FFF)
			{
				nowValue = nowValue + 150;
				nowWordValue = nowWordValue + 150;
				newDB->wordValueList.push_back(nowWordValue);
				newDB->altText = newDB->altText + text[i];
				nowWordValue = 0;
				nowTokenValue++;

			}
		}

		if (tokenSize == 1)
		{
			newDB->wordValueList.push_back(nowWordValue);
		}

		newDB->textLength = newDB->altText.length();
		newDB->originalText = text;
		newDB->tokenValue = tokenSize;
		newDB->value = nowValue;
	}
}

//완전히 매칭 되는 단어로 검색.
std::wstring MainCore::GetMatchingSpellingCheck(std::wstring text, bool* isReplaceFlag)
{
	//std::wcout << text << L"End?";
	std::wstring resultSTring = L"";

	StringTokenizer tokens(text, L" ");
	std::wstring tokenString;
	int addIndex = 1;
	int tokenSize = tokens.countTokens();
	int nowTokenCount = 0;
	bool isReplaceWordFlag = false;
	std::wstring* replaceText;
	std::wstring debugText = L"";


	//std::wcout << "step1 " << text << std::endl;
	std::vector<std::wstring> textList;

	for (int x = 1; tokens.hasMoreTokens(); x++)
	{
		textList.push_back(tokenString = tokens.nextToken());
	}

	//std::cout << "step 2 token size : " << tokenSize;
	std::pair <std::multimap<std::wstring, DicDB>::iterator, std::multimap<std::wstring, DicDB>::iterator> mapPD;
	for (int i = 0; i < tokenSize; )			//원문 갯수 만큼 돌림
	{
		//std::cout << "step3 " << i << std::endl;
		mapPD = myDicMap.equal_range(textList[i]);
		replaceText = &textList[i];

		for (std::multimap<std::wstring, DicDB>::iterator it = mapPD.first; it != mapPD.second; ++it)
		{
			debugText = L"";
			//std::wcout << "step4 " << it->second.originalText << std::endl;
			if (it->second.tokenSize > nowTokenCount)
			{
				int textSize = textList.max_size();
				int textListSize = it->second.originalTextList.size();

				for (int j = 0; j < it->second.tokenSize && i + j < textSize && j < textListSize; j++)
				{

					if (tokenSize < i + it->second.tokenSize)
					{
						isReplaceWordFlag = false;
						break;
					}

					if (it->second.originalTextList[j].compare(textList[i + j]) == 0)
					{
						if (debugMode.isActive && debugMode.isShowReplace)
						{
							debugText = debugText + textList[i + j] + L" ";
						}

						isReplaceWordFlag = true;
					}
					else
					{
						isReplaceWordFlag = false;
						break;
					}
				}
			}
			if (isReplaceWordFlag == true)
			{

				(*isReplaceFlag) = true;
				nowTokenCount = it->second.tokenSize;
				if (debugMode.isActive && debugMode.isShowReplace)
				{
					debugText = L"[" + debugText + L"]";
					replaceText = &it->second.resultText;

				}
				else
				{
					replaceText = &it->second.resultText;
				}

				isReplaceWordFlag = false;
			}
		}
		if (nowTokenCount > 1)
		{
			i = i + nowTokenCount;
		}
		else
		{
			i++;
		}

		if (debugMode.isActive && debugMode.isShowReplace)
		{
			resultSTring = resultSTring + (*replaceText) + debugText;
			debugText = L"";
		}
		else
		{
			resultSTring = resultSTring + (*replaceText);
		}

		if (i < tokenSize)
		{
			resultSTring = resultSTring + L" ";
		}


		nowTokenCount = 0;
	}

	return resultSTring;
}

//문자 단위로 체크.
std::wstring MainCore::GetLetterSpellingCheck(std::wstring text, bool* isReplaceFlag)
{


	std::wstring resultSTring = L"";
	std::wstring debugText = L"";

	std::wstring replaceText;
	int nowTokenCount = 0;
	int tokenSize = text.length();
	bool isReplaceWordFlag = true;
	std::pair <std::multimap<std::wstring, DicDB>::iterator, std::multimap<std::wstring, DicDB>::iterator> mapPD;
	std::wstring tokenString = L"";


	for (int i = 0; i < tokenSize; )			//원문 갯수 만큼 돌림
	{
		tokenString = text[i];
		mapPD = myDicMap.equal_range(tokenString);
		replaceText = text[i];

		debugText = L"";
		for (std::multimap<std::wstring, DicDB>::iterator it = mapPD.first; it != mapPD.second; ++it)
		{

			if (it->second.tokenSize > nowTokenCount)
			{
				for (int j = 0; j < it->second.tokenSize; j++)
				{
					if (tokenSize < i + it->second.tokenSize)
					{
						isReplaceWordFlag = false;
						break;
					}
					std::wstring text1 = L"";
					text1 = it->second.originalText[j];
					std::wstring text2 = L"";
					text2 = text[i + j];

					if (text1.compare(text2) == 0)
					{

						if (debugMode.isActive && debugMode.isShowReplace)
						{
							debugText = debugText + text2;

						}

						isReplaceWordFlag = true;
					}
					else
					{
						isReplaceWordFlag = false;
						break;
					}
				}
			}
			if (isReplaceWordFlag == true)
			{
				(*isReplaceFlag) = true;
				nowTokenCount = it->second.tokenSize;



				if (debugMode.isActive && debugMode.isShowReplace)
				{
					debugText = L"[" + debugText + L"]";
					replaceText = it->second.resultText;

				}
				else
				{
					replaceText = it->second.resultText;
				}

				isReplaceWordFlag = false;
			}

		}
		if (nowTokenCount > 1)
		{
			i = i + nowTokenCount;
		}
		else
		{
			i++;
		}

		if (debugMode.isActive && debugMode.isShowReplace)
		{
			resultSTring = resultSTring + (replaceText)+debugText;
			debugText = L"";
		}
		else
		{
			resultSTring = resultSTring + (replaceText);
		}


		nowTokenCount = 0;
	}

	return resultSTring;
}

//교정단어 검색
std::wstring MainCore::checkSpelling(std::wstring text, bool* isReplaceFlag, std::wstring toekn)
{
	clock_t start, end;

	start = clock();
	std::wstring result = L"";


	int count = reCehckSpllingCount + 1;
	*isReplaceFlag = false;
	

	for (int i = 0; i < count; i++)
	{
		std::wstring resultString = L"";
		bool isReplace = false;
		StringTokenizer tokens(text, toekn);

		for (int x = 1; tokens.hasMoreTokens(); x++)
		{
			std::wstring splite = tokens.nextToken();

			if (splite.compare(L"\r\n") == 1)
			{
				if (isUseMatchWordDic)
				{
					resultString = resultString + GetMatchingSpellingCheck(splite, &isReplace) + L"\r\n";
				}
				else
				{
					resultString = resultString + GetLetterSpellingCheck(splite, &isReplace) + L"\r\n";;
				}
			}			
		}

		result = resultString;

		if (isReplace)
		{
			std::cout << std::endl << "Spell Change : " << i <<  " / " << count << std::endl;
			*isReplaceFlag = true;
			text = resultString;
		}
		else
		{
			std::cout << std::endl << "Spell finish : " << i << std::endl;
			break;
		}
	}

	end = clock();

	double  time = (end - start);
	std::cout << std::endl << "Check Spelling Time : " << time << std::endl;

	return result;

}


void MainCore::ReadMatchingDicFile(char* dicFileText)
{
	bool endFileFlag = false;
	char getLine[1000];


	std::ifstream fin;


	std::string getString;
	char setDICFileName[100] = ".\\DIC\\";
	std::strcat(setDICFileName, dicFileText);

	fin.open(setDICFileName);
	if (fin.fail())
	{
		return;
	}



	while (!fin.eof())
	{
		fin.getline(getLine, 1000);

		if (getLine[0] == '/')
		{
			if (getLine[1] == 's')	//시작을 알림
			{
				DicDB newDB = DicDB();
				getString = "";

				fin.getline(getLine, 1000);
				getString = getLine;
				std::wstring wGetString = stringToWstring(getString);

				newDB.originalText = wGetString;


				fin.getline(getLine, 1000);
				getString = getLine;
				wGetString = stringToWstring(getString);
				newDB.resultText = wGetString;

				StringTokenizer tokens(newDB.originalText, L" ");
				std::wstring tokenString;
				newDB.tokenSize = tokens.countTokens();

				for (int x = 1; tokens.hasMoreTokens(); x++)
				{
					tokenString = tokens.nextToken();
					newDB.originalTextList.push_back(tokenString);
				}

				myDicMap.insert(std::pair<std::wstring, DicDB>(newDB.originalTextList.front(), newDB));


			}
		}

	}
	fin.close();
}

void MainCore::ReadUnMatchingDicFile(char* dicFileText)
{
	bool endFileFlag = false;
	char getLine[1000];

	std::ifstream fin;


	std::string getString;
	char setDICFileName[100] = ".\\DIC\\";

	std::strcat(setDICFileName, dicFileText);
	fin.open(setDICFileName);
	if (fin.fail())
	{
		return;
	}

	std::wstring wGetLinestring = L"";

	while (!fin.eof())
	{
		fin.getline(getLine, 1000);
		std::string getLineString = getLine;
		wGetLinestring = utfStringToWstring(getLineString);

		if (wGetLinestring[0] == '/')
		{
			if (wGetLinestring[1] == 's')
			{
				DicDB* newDB = new DicDB();
				getString = "";
				fin.getline(getLine, 1000);
				getString = getLine;
				std::wstring wGetstring = utfStringToWstring(getString);
				newDB->originalText = wGetstring;
				newDB->tokenSize = wGetstring.length();

				fin.getline(getLine, 1000);
				getString = getLine;

				wGetstring = utfStringToWstring(getString);
				newDB->resultText = wGetstring;

				std::wstring tokenString = L"";
				tokenString = newDB->originalText[0];


				myDicMap.insert(std::pair<std::wstring, DicDB>(tokenString, *newDB));

			}
		}
	}
	fin.close();
}
//교정단어 사전 불러오기
void MainCore::openDicFile(char* dicFileText)
{
	bool endFileFlag = false;
	char getLine[1000];

	std::ifstream fin;


	std::string getString;
	myDicMap.clear();

	if (isUseJpnFlag == false)
	{
		ReadMatchingDicFile("dic.txt");
		if (strcmp(dicFileText, "") != 0 && strcmp(dicFileText, "dic.txt") != 0 && strcmp(dicFileText, "dicJpn.txt") != 0)
		{
			if (isUseMatchWordDic)
			{
				ReadMatchingDicFile(dicFileText);
			}
			else

			{
				ReadUnMatchingDicFile(dicFileText);
			}

		}

	}
	else if (isUseJpnFlag == true)
	{
		ReadUnMatchingDicFile("dicJpn.txt");
		if (strcmp(dicFileText, "") != 0 && strcmp(dicFileText, "dic.txt") != 0 && strcmp(dicFileText, "dicJpn.txt") != 0)
		{
			if (isUseMatchWordDic)
			{
				ReadMatchingDicFile(dicFileText);
			}
			else

			{
				ReadUnMatchingDicFile(dicFileText);
			}
		}


	}


}

int isUTF8(const char* data, size_t size)
{
	const unsigned char* str = (unsigned char*)data;
	const unsigned char* end = str + size;
	unsigned char byte;
	unsigned int code_length, i;
	uint32_t ch;
	while (str != end) {
		byte = *str;
		if (byte <= 0x7F) {
			/* 1 byte sequence: U+0000..U+007F */
			str += 1;
			continue;
		}

		if (0xC2 <= byte && byte <= 0xDF)
			/* 0b110xxxxx: 2 bytes sequence */
			code_length = 2;
		else if (0xE0 <= byte && byte <= 0xEF)
			/* 0b1110xxxx: 3 bytes sequence */
			code_length = 3;
		else if (0xF0 <= byte && byte <= 0xF4)
			/* 0b11110xxx: 4 bytes sequence */
			code_length = 4;
		else {
			/* invalid first byte of a multibyte character */
			return 0;
		}

		if (str + (code_length - 1) >= end) {
			/* truncated string or invalid byte sequence */
			return 0;
		}

		/* Check continuation bytes: bit 7 should be set, bit 6 should be
		 * unset (b10xxxxxx). */
		for (i = 1; i < code_length; i++) {
			if ((str[i] & 0xC0) != 0x80)
				return 0;
		}

		if (code_length == 2) {
			/* 2 bytes sequence: U+0080..U+07FF */
			ch = ((str[0] & 0x1f) << 6) + (str[1] & 0x3f);
			/* str[0] >= 0xC2, so ch >= 0x0080.
			   str[0] <= 0xDF, (str[1] & 0x3f) <= 0x3f, so ch <= 0x07ff */
		}
		else if (code_length == 3) {
			/* 3 bytes sequence: U+0800..U+FFFF */
			ch = ((str[0] & 0x0f) << 12) + ((str[1] & 0x3f) << 6) +
				(str[2] & 0x3f);
			/* (0xff & 0x0f) << 12 | (0xff & 0x3f) << 6 | (0xff & 0x3f) = 0xffff,
			   so ch <= 0xffff */
			if (ch < 0x0800)
				return 0;

			/* surrogates (U+D800-U+DFFF) are invalid in UTF-8:
			   test if (0xD800 <= ch && ch <= 0xDFFF) */
			if ((ch >> 11) == 0x1b)
				return 0;
		}
		else if (code_length == 4) {
			/* 4 bytes sequence: U+10000..U+10FFFF */
			ch = ((str[0] & 0x07) << 18) + ((str[1] & 0x3f) << 12) +
				((str[2] & 0x3f) << 6) + (str[3] & 0x3f);
			if ((ch < 0x10000) || (0x10FFFF < ch))
				return 0;
		}
		str += code_length;
	}
	return 1;
}




//문자값 / DB 파일 / 기본 설정 불러오기
void MainCore::openSettingFile(char* dbFileName)
{

	if (isUseDBFlag == true)
	{
		std::cout << "\n\nopen db file and is jpn? : " << isUseJpnFlag << "\n";

		int nowDBIndex = 0;
		if (isUseJpnFlag == false)
		{
			bool endFileFlag = false;
			char getLine[PER_GET_TEXT];

			std::ifstream fin;
			fin.open(L"characterValue.txt");

			int temp = 0;
			int nowIndex = 0;
			int nowIndex2 = 0;

			lastDBIndex = 0;
			myTranslationVector.clear();
			myTranslationMap.clear();

			while (!fin.eof())
			{
				fin.getline(getLine, PER_GET_TEXT);
				myCharacterTable.characteValue[nowIndex][nowIndex2++] = atoi(getLine);

				fin.getline(getLine, PER_GET_TEXT);
				myCharacterTable.characteValue[nowIndex][nowIndex2++] = atoi(getLine);

				fin.getline(getLine, PER_GET_TEXT);
				myCharacterTable.characteValue[nowIndex][nowIndex2++] = atoi(getLine);

				nowIndex++;
				nowIndex2 = 0;
			}
			fin.close();

			std::wstring wGetString = L"";
			char setDBFileName[100] = ".\\DB\\";

			std::strcat(setDBFileName, dbFileName);
			fin.open(setDBFileName, std::ios::binary);
			TranslationsDB newDB;
			if (fin.is_open() == true)
			{
				char* buffer;
				fin.seekg(0, std::ios::end);
				int length = fin.tellg();
				fin.seekg(0, std::ios::beg);
				buffer = new char[length];


				fin.read(buffer, length);
				int utf = isUTF8(buffer, length);


				delete[] buffer;
				fin.close();
				fin.open(setDBFileName);

				while (!fin.eof())
				{

					//fin.getline(getLine, PER_GET_TEXT);
					//std::string getLineString = getLine;

					std::string getLineString;
					std::getline(fin, getLineString);
					std::wstring wGetLinestring;

					if (utf == 0)
					{
						wGetLinestring = stringToWstring(getLineString);
					}
					else
					{
						wGetLinestring = utfStringToWstring(getLineString);
					}
					if (getLineString[0] == '/')
					{
						if (getLineString[1] == 's')	//시작을 알림
						{
							newDB = TranslationsDB();
							wGetString = L"";
						}
						else if (getLineString[1] == 't')	//번역본 시작을 알림. 고로 오리지날 분석
						{
							analysisText(wGetString, &newDB);
							wGetString = L"";

						}
						else if (getLineString[1] == 'e')	//다 끝남
						{
							newDB.translationText = wGetString;
							newDB.index = nowDBIndex++;
							wGetString = L"";
							myTranslationMap.insert(std::pair<int, TranslationsDB>(newDB.tokenValue, newDB));
							myTranslationVector.push_back(newDB);
						}
					}
					else
					{
						wGetString += wGetLinestring;
						//wGetString += L"\r\n";
					}
				}
			}
			fin.close();
		}
		else if (isUseJpnFlag == true)
		{
			bool endFileFlag = false;
			char getLine[PER_GET_TEXT];

			std::ifstream fin;

			int temp = 0;
			int nowIndex = 0;
			int nowIndex2 = 0;
			std::wstring wGetString = L"";

			myTranslationVector.clear();
			myTranslationMap.clear();
			char setDBFileName[100] = ".\\DB\\";

			std::strcat(setDBFileName, dbFileName);
			fin.open(setDBFileName);
			TranslationsDB newDB;

			if (fin.is_open() == true)
			{
				while (!fin.eof())
				{
					std::string getLineString;
					std::getline(fin, getLineString);

					std::wstring wGetLinestring = utfStringToWstring(getLineString);
					if (wGetLinestring[0] == '/')
					{
						if (wGetLinestring[1] == 's')	//시작을 알림
						{
							newDB = TranslationsDB();
							wGetString = L"";
						}
						else if (wGetLinestring[1] == 't')	//번역본 시작을 알림. 고로 오리지날 분석
						{
							analysisText(wGetString, &newDB);
							wGetString = L"";

						}
						else if (wGetLinestring[1] == 'e')	//다 끝남
						{
							newDB.translationText = wGetString;
							newDB.index = nowDBIndex++;
							wGetString = L"";
							myTranslationMap.insert(std::pair<int, TranslationsDB>(newDB.tokenValue, newDB));
							myTranslationVector.push_back(newDB);				

						}
					}
					else
					{
						wGetString += wGetLinestring;
						wGetString += L"\r\n";
					}
				}
			}

			fin.close();
		}


	}
}


void MainCore::initScreenCapture()
{

}

//색 그룹 추가
void MainCore::AddOcrColorSet(int colorList[], int size)
{
	std::vector<int> vector;
	for (int i = 0; i < size; i++)
	{
		vector.push_back(colorList[i]);
	}

	useColorSetList.push_back(vector);




}

//색 그룹 초기화
void MainCore::ClearOcrColorSet()
{
	for (int i = 0; i < useColorSetList.size(); i++)
	{
		useColorSetList[i].clear();
	}

	useColorSetList.clear();


}

//추출 좌표 설정
void MainCore::setCutPoint(int newX[], int newY[], int  newWidth[], int newHeight[], int newSize)
{
	//모니터 dpi 비율
	float scaleX = 1;
	float scaleY = 1;

	/*
	HWND hwnd=GetDesktopWindow();

	if(hwnd != NULL)
	{
		int tempX = 1;
		int tempY = 1;
		RECT rc;
		GetClientRect(hwnd, &rc);

		tempX =rc.right;
		tempY = rc.bottom;

		HDC hdcScreen = GetDC(hwnd);
		GetClipBox(hdcScreen, &rc);




		scaleX =(float)rc.right / tempX;
		scaleY = (float)rc.bottom / tempY;
		ReleaseDC(NULL, hdcScreen);
	}
	*/

	RECT rc;	//모니터 사이즈를 가져오는 부분
	rc.left = GetSystemMetrics(SM_CXFOCUSBORDER);
	rc.top = GetSystemMetrics(SM_CYFOCUSBORDER);
	rc.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rc.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	HDC hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	GetClipBox(hdcScreen, &rc);

	int startPointX = rc.left;
	int startPointY = rc.top;
	int width2 = rc.right - rc.left;
	int height2 = rc.bottom - rc.top;

	if (width != 0 || height != 0)
	{
		scaleX = (float)(width2) / (float)(width);
		scaleY = (float)(height2) / (float)(height);
	}
	else
	{
		scaleX = 1;
		scaleY = 1;
	}

	if (scaleX == 0)
	{
		scaleX = 1;
	}
	if (scaleY == 0)
	{
		scaleY = 1;
	}
	ReleaseDC(NULL, hdcScreen);
	cutCodinateXList.clear();
	cutCodinateYList.clear();
	cutHeightList.clear();
	cutWidthList.clear();
	for (int i = 0; i < newSize; i++)
	{
		cutCodinateXList.push_back((int)(newX[i] * scaleX));
		cutCodinateYList.push_back((int)(newY[i] * scaleY));
		cutHeightList.push_back((int)(newHeight[i] * scaleY));
		cutWidthList.push_back((int)(newWidth[i] * scaleX));
	}

}



//제외 영역 추가.
void MainCore::SetExceptPoint(int newX[], int newY[], int  newWidth[], int newHeight[], int newSize)
{
	//모니터 dpi 비율
	float scaleX = 1;
	float scaleY = 1;


	RECT rc;	//모니터 사이즈를 가져오는 부분
	rc.left = GetSystemMetrics(SM_CXFOCUSBORDER);
	rc.top = GetSystemMetrics(SM_CYFOCUSBORDER);
	rc.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rc.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	HDC hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	GetClipBox(hdcScreen, &rc);

	int startPointX = rc.left;
	int startPointY = rc.top;
	int width2 = rc.right - rc.left;
	int height2 = rc.bottom - rc.top;


	if (width != 0 || height != 0)
	{
		scaleX = (float)(width2) / (float)(width);
		scaleY = (float)(height2) / (float)(height);
	}
	else
	{
		scaleX = 1;
		scaleY = 1;
	}

	if (scaleX == 0)
	{
		scaleX = 1;
	}
	if (scaleY == 0)
	{
		scaleY = 1;
	}
	ReleaseDC(NULL, hdcScreen);
	exceptCodinateXList.clear();
	exceptCodinateYList.clear();
	exceptHeightList.clear();
	exceptWidthList.clear();
	for (int i = 0; i < newSize; i++)
	{
		exceptCodinateXList.push_back((int)(newX[i] * scaleX));
		exceptCodinateYList.push_back((int)(newY[i] * scaleY));
		exceptHeightList.push_back((int)(newHeight[i] * scaleY));
		exceptWidthList.push_back((int)(newWidth[i] * scaleX));
	}

}


//이미지 보정
void MainCore::adjustImg(cv::Mat* img, int captureIndex)
{
	bool isRequireCheck = false;		//색 그룹을 전혀 사용하지 않는 ocr은 무시한다.

	if (isRGBOptionFlag || isHSVOptionFlag || isThresHold)
	{
		for (int i = 0; i < useColorSetList[captureIndex].size(); i++)
		{
			if (useColorSetList[captureIndex][i] == 1)
			{
				isRequireCheck = true;
				break;
			}
		}
	}


	//fiducialH
	if (isRequireCheck && isRGBOptionFlag == true)
	{
		for (int i = 0; i < img->rows; i++)
		{
			for (int j = 0; j < img->cols; j++)
			{
				int b = img->at<cv::Vec4b>(i, j)[0];
				int g = img->at<cv::Vec4b>(i, j)[1];
				int r = img->at<cv::Vec4b>(i, j)[2];

				bool isInRangeFlag = false;
				//debugStruct.debug = L"";
				for (int y = 0; y < fiducialValuseList.size(); y++)
				{

					//debugStruct.debug.append(L"useColorSetList[0][");
					//debugStruct.debug.append(std::to_wstring(y));
					//debugStruct.debug += L"] ";
					//debugStruct.debug.append(std::to_wstring(useColorSetList[captureIndex][y]));
					if (useColorSetList[captureIndex][y] == 1)
					{
						if ((r == fiducialValuseList[y].fiducialR) && (g == fiducialValuseList[y].fiducialG) && (b == fiducialValuseList[y].fiducialB))
						{
							img->at<cv::Vec4b>(i, j)[0] = 0;
							img->at<cv::Vec4b>(i, j)[1] = 0;
							img->at<cv::Vec4b>(i, j)[2] = 0;
							img->at<cv::Vec4b>(i, j)[3] = 255;
							isInRangeFlag = true;
							break;
						}
					}

				}
				if (isInRangeFlag == false)
				{
					img->at<cv::Vec4b>(i, j)[0] = 255;
					img->at<cv::Vec4b>(i, j)[1] = 255;
					img->at<cv::Vec4b>(i, j)[2] = 255;
					img->at<cv::Vec4b>(i, j)[3] = 255;
				}
			}
		}
	}
	else if (isRequireCheck && isHSVOptionFlag == true)
	{
		cv::Mat hsv;
		cvtColor(*img, hsv, 40);	//CV_BGR2HSV
		for (int i = 0; i < img->rows; i++)
		{
			for (int j = 0; j < img->cols; j++)
			{

				INT32 s = hsv.at<cv::Vec3b>(i, j)[1] * 100 / 255;
				INT32 v = hsv.at<cv::Vec3b>(i, j)[2] * 100 / 255;

				bool isInRangeFlag = false;

				for (int y = 0; y < fiducialValuseList.size(); y++)
				{
					if (useColorSetList[captureIndex][y] == 1)
					{
						if ((fiducialValuseList[y].fiducialSS <= s && fiducialValuseList[y].fiducialES >= s) &&
							(fiducialValuseList[y].fiducialSV <= v && fiducialValuseList[y].fiducialEV >= v))
						{
							img->at<cv::Vec4b>(i, j)[0] = 0;
							img->at<cv::Vec4b>(i, j)[1] = 0;
							img->at<cv::Vec4b>(i, j)[2] = 0;
							img->at<cv::Vec4b>(i, j)[3] = 255;
							isInRangeFlag = true;
							break;

						}
					}
				}
				if (isInRangeFlag == false)
				{
					img->at<cv::Vec4b>(i, j)[0] = 255;
					img->at<cv::Vec4b>(i, j)[1] = 255;
					img->at<cv::Vec4b>(i, j)[2] = 255;
					img->at<cv::Vec4b>(i, j)[3] = 255;
				}
			}
		}

		hsv.release();
	}
	else if (isRequireCheck && isThresHold)
	{
		double value = thresholdValue;
		cvtColor(*img, *img, 6);	//CV_BGR2GRAY
		threshold(*img, *img, value, 255, cv::ThresholdTypes::THRESH_BINARY);
	}
	else
	{
		cvtColor(*img, *img, 6);	//CV_BGR2GRAY
		//cvtColor(*img, *img, CV_GRAY2BGRA);
	}

	if (isErodeOptionFlag == true)
	{
		if (isHSVOptionFlag || isRGBOptionFlag)
		{
			*img = ~*img;
			//cvtColor(*img, *img, 6);	//CV_BGR2GRAY
		}
		cv::erode(*img, *img, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
		/*
		cv::Mat mask;
		img->copyTo(mask);


		cv::Mat element = getStructuringElement( cv::MORPH_RECT, cv::Size( 2+1, 2+1 ) );
		cv::erode(mask, mask, getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6)));
		cv::dilate(mask, mask, getStructuringElement(cv::MORPH_RECT, cv::Size(24,24)));


		cv::bitwise_not(*img, *img, mask);
		mask.release();
		*/

		if (isHSVOptionFlag || isRGBOptionFlag)
		{
			*img = ~*img;
		}



		//오리지날
		/*
		cv::Mat element = getStructuringElement(cv::MORPH_RECT,
			cv::Size(3, 3),
			cv::Point(2, 2));
		cv::erode(*img, *img, element);
		*/



	}



}

//제외 영역 처리
void MainCore::RemoveAreaImg(cv::Mat* newImg, int captureIndex)
{
	int oriStartX = cutCodinateXList[captureIndex];
	int oriEndX = oriStartX + cutWidthList[captureIndex];

	int oriStartY = cutCodinateYList[captureIndex];
	int oriEndY = oriStartY + cutHeightList[captureIndex];


	int exceptCount = exceptCodinateXList.size();

	if (exceptCount > 0)
	{
		for (int i = 0; i < exceptCount; i++)
		{
			int exStartX = exceptCodinateXList[i];
			int exEndX = exStartX + exceptWidthList[i];


			int exStartY = exceptCodinateYList[i];
			int exEndY = exStartY + exceptHeightList[i];

			if (exStartX > oriEndX || exEndX < oriStartX || exStartY > oriEndY || exEndY < oriStartY)
			{

			}
			else
			{

				RECT target = { 0,0,0,0 };

				target.left = exStartX - oriStartX;
				target.right = target.left + exceptWidthList[i];
				target.bottom = exStartY - oriStartY;
				target.top = target.bottom + exceptHeightList[i];


				if (target.left < 0)
				{
					target.left = 0;
				}

				if (target.right > newImg->cols)
				{
					target.right = newImg->cols;
				}


				if (target.bottom < 0)
				{
					target.bottom = 0;
				}

				if (target.top > newImg->rows)
				{
					target.top = newImg->rows;
				}


				cv::Scalar white = cv::Scalar(255, 255, 255);
				cv::Rect rect(target.left, target.bottom, (target.right - target.left), (target.top - target.bottom));
				cv::rectangle(*newImg, rect, white, -1);

			}

		}
	}

}

//화면 가져오기
void MainCore::getScreen(cv::Mat* newImg, int captureIndex, int* locationX, int* locationY)
{

	std::wcout << std::endl << L" Start Get Screen" << std::endl;

	bool isError = false;
	RECT rc, clientCoordinate;

	rc = { 0,0,0,0 };

	clientCoordinate.left = 0;
	clientCoordinate.top = 0;
	HDC hdcScreen = NULL;
	if (!isActiveWindow)
	{
		hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	}

	else
	{
		//hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
		//엑티브 윈도우
		HWND firstHwnd = GetForegroundWindow();
		hdcScreen = GetWindowDC(firstHwnd);
		GetClipBox(hdcScreen, &rc);
		GetWindowRect(firstHwnd, &clientCoordinate);


		if (rc.bottom == 0 && rc.right == 0)
		{
			std::cout << "We Doom";
			std::cout << " rc : " << rc.left << " / " << rc.right << " + " << rc.top << " / " << rc.bottom << " top : " << cutCodinateYList[captureIndex] << " cut : " << cutHeightList[captureIndex] << "\n";

			HWND hwnd = GetDesktopWindow();
			GetClientRect(hwnd, &rc);
			isError = true;
		}
		else
		{
			*locationX = clientCoordinate.left;
			*locationY = clientCoordinate.top;
		}

	}



	HDC hdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,
		cutWidthList[captureIndex], cutHeightList[captureIndex]);
	SelectObject(hdc, hbmp);

	int height, width, srcheight, srcwidth;


	myBitmapHeader.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	//myBitmapHeader.biWidth = windowsize.right;    
	//myBitmapHeader.biHeight = -windowsize.bottom;  //this is the line that makes it draw upside down or not

	myBitmapHeader.biWidth = cutWidthList[captureIndex];
	myBitmapHeader.biHeight = -cutHeightList[captureIndex];  //this is the line that makes it draw upside down or not

	myBitmapHeader.biPlanes = 1;
	myBitmapHeader.biBitCount = 32;	//알파 = 32    
	myBitmapHeader.biCompression = BI_RGB;
	myBitmapHeader.biSizeImage = 0;
	myBitmapHeader.biXPelsPerMeter = 0;
	myBitmapHeader.biYPelsPerMeter = 0;
	myBitmapHeader.biClrUsed = 0;
	myBitmapHeader.biClrImportant = 0;



	if (isActiveWindow && !isError)
	{
		int xStart = cutCodinateXList[captureIndex] - clientCoordinate.left;
		int xEnd = xStart + cutWidthList[captureIndex];

		int yStart = cutCodinateYList[captureIndex] - clientCoordinate.top;
		int yEnd = yStart + cutHeightList[captureIndex];

		int _width = cutWidthList[captureIndex];
		int _height = cutHeightList[captureIndex];

		_width = clientCoordinate.right - clientCoordinate.left;
		_height = clientCoordinate.bottom - clientCoordinate.top;
		if (xStart < 0)
		{
			xStart = 0;
		}
		else if (xStart > _width)
		{
			xStart = _width - 1;
		}


		if (xEnd > _width)
		{
			xEnd = _width;
		}
		else if (xEnd <= 0)
		{
			xEnd = 1;
		}


		if (yStart < 0)
		{
			yStart = 0;
		}
		else if (yStart > _height)
		{
			yStart = _height - 1;
		}

		if (yEnd > _height)
		{
			yEnd = _height;
		}
		else if (yEnd <= 0)
		{
			yEnd = 1;
		}

		_height = yEnd - yStart;
		_width = xEnd - xStart;

		if (_height <= 0)
		{
			_height = 1;
		}

		if (_width <= 0)
		{
			_width = 1;
		}


		//std::cout << " _width " << _width << " _Height " << _height << " / x start : " << xStart << " end : " << xEnd  << " Y start : " << yStart << " Y end : " << yEnd;
		myBitmapHeader.biWidth = _width;
		myBitmapHeader.biHeight = -_height;  //this is the line that makes it draw upside down or not


		newImg->create(_height, _width, CV_8UC4);
		StretchBlt(hdc, 0, 0, _width, _height, hdcScreen, xStart, yStart, _width, _height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
		//CV_8UC4
		GetDIBits(hdc, hbmp, 0, _height, newImg->data, (BITMAPINFO*)&myBitmapHeader, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	}
	else
	{
		newImg->create(cutHeightList[captureIndex], cutWidthList[captureIndex], CV_8UC4);
		StretchBlt(hdc, 0, 0, cutWidthList[captureIndex], cutHeightList[captureIndex], hdcScreen, cutCodinateXList[captureIndex] - clientCoordinate.left, cutCodinateYList[captureIndex] - clientCoordinate.top, cutWidthList[captureIndex], cutHeightList[captureIndex], SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
		//CV_8UC4
		GetDIBits(hdc, hbmp, 0, cutHeightList[captureIndex], newImg->data, (BITMAPINFO*)&myBitmapHeader, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	}




	if (debugMode.isActive && debugMode.isSaveCapture)
	{
		//원본 캡쳐
		cv::imwrite("capture_Original.bmp", *newImg);
	}


	DeleteDC(hdc);
	DeleteObject(hbmp);
	ReleaseDC(NULL, hdcScreen);;

	//if(isAdvencedIMGOptionFlag == true)
	adjustImg(newImg, captureIndex);
	RemoveAreaImg(newImg, captureIndex);


	resize(*newImg, *newImg, cv::Size(), imgZoomSize, imgZoomSize, cv::INTER_LINEAR);

	if (debugMode.isActive && debugMode.isSaveCaptureResult)
	{
		//테스트용
		cv::imwrite("capture_Result.bmp", *newImg);
	}
}



//화면 가져오기
void MainCore::getScreen(cv::Mat* newImg, int captureIndex)
{
	int locationX = 0;
	int locationY = 0;
	getScreen(newImg, captureIndex, &locationX, &locationY);

}



//화면 가져오기
void MainCore::getScreen2(cv::Mat* newImg, int captureIndex, uint8_t* bytes, int _width, int _height, int positionX, int positionY)
{

	int xStart = cutCodinateXList[captureIndex] - positionX;
	int xEnd = xStart + cutWidthList[captureIndex];

	int yStart = cutCodinateYList[captureIndex] - positionY;
	int yEnd = yStart + cutHeightList[captureIndex];

	if (xStart < 0)
	{
		xStart = 0;
	}
	else if (xStart > _width)
	{
		xStart = _width - 1;
	}


	if (xEnd > _width)
	{
		xEnd = _width;
	}
	else if (xEnd <= 0)
	{
		xEnd = 1;
	}


	if (yStart < 0)
	{
		yStart = 0;
	}
	else if (yStart > _height)
	{
		yStart = _height - 1;
	}

	if (yEnd > _height)
	{
		yEnd = _height;
	}
	else if (yEnd <= 0)
	{
		yEnd = 1;
	}

	std::cout << "x start " << xStart << " / xEnd " << xEnd << " / yStart " << yStart << " / yEnd " << yEnd;

	cv::Mat targetImg = cv::Mat(_height, _width, CV_8UC4, bytes);
	cv::Mat targetImg2 = targetImg(cv::Range::Range(yStart, yEnd), cv::Range::Range(xStart, xEnd));

	//CV_8UC4
	//newImg->create(cutHeightList[captureIndex], cutWidthList[captureIndex], CV_8UC4);
	targetImg2.copyTo(*newImg);

	if (debugMode.isActive && debugMode.isSaveCapture)
	{
		//원본 캡쳐
		cv::imwrite("capture_Original.bmp", *newImg);
	}



	//if(isAdvencedIMGOptionFlag == true)
	adjustImg(newImg, captureIndex);
	RemoveAreaImg(newImg, captureIndex);


	resize(*newImg, *newImg, cv::Size(), imgZoomSize, imgZoomSize, cv::INTER_LINEAR);

	if (debugMode.isActive && debugMode.isSaveCaptureResult)
	{
		//테스트용
		cv::imwrite("capture_Result.bmp", *newImg);
	}

	targetImg.release();
	targetImg2.release();
}