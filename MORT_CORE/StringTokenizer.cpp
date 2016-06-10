/*
 ***********************************************************************
 * Note: This library has been deprecated in favour of the C++ String  *
 * Toolkit Library (StrTk).                                            *
 * URL: http://www.partow.net/programming/strtk/index.html             *
 ***********************************************************************
*/

#include "StringTokenizer.h"

StringTokenizer::StringTokenizer(const std::wstring& _str, const std::wstring& _delim)
{
   if ((_str.length() == 0) || (_delim.length() == 0)) return;

   token_str = _str;
   delim     = _delim;

   /*
     Remove sequential delimiter
   */
   unsigned int curr_pos = 0;

   while(true)
   {
      if ((curr_pos = token_str.find(delim,curr_pos)) != std::wstring::npos)
      {
         curr_pos += delim.length();

         while(token_str.find(delim,curr_pos) == curr_pos)
         {
            token_str.erase(curr_pos,delim.length());
         }
      }
      else
       break;
   }

   /*
     Trim leading delimiter
   */
   if (token_str.find(delim,0) == 0)
   {
      token_str.erase(0,delim.length());
   }

   /*
     Trim ending delimiter
   */
   curr_pos = 0;
   if ((curr_pos = token_str.rfind(delim)) != std::wstring::npos)
   {
      if (curr_pos != (token_str.length() - delim.length())) return;
      token_str.erase(token_str.length() - delim.length(),delim.length());
   }
}

int StringTokenizer::countTokens()
{
   unsigned int prev_pos = 0;
   int num_tokens        = 0;

   if (token_str.length() > 0)
   {
      num_tokens = 0;

      unsigned int curr_pos = 0;
      while(true)
      {
         if ((curr_pos = token_str.find(delim,curr_pos)) != std::wstring::npos)
         {
            num_tokens++;
            prev_pos  = curr_pos;
            curr_pos += delim.length();
         }
         else
          break;
      }
      return ++num_tokens;
   }
   else
   {
      return 0;
   }
}

bool StringTokenizer::hasMoreTokens()
{
   return (token_str.length() > 0);
}

std::wstring StringTokenizer::nextToken()
{
   if (token_str.length() == 0)
     return L"";

   std::wstring  tmp_str = L"";
   unsigned int pos     = token_str.find(delim,0);

   if (pos != std::wstring::npos)
   {
      tmp_str   = token_str.substr(0,pos);
      token_str = token_str.substr(pos+delim.length(),token_str.length()-pos);
   }
   else
   {
      tmp_str   = token_str.substr(0,token_str.length());
      token_str = L"";
   }
   return tmp_str;
}

int StringTokenizer::nextIntToken()
{
   return _wtoi(nextToken().c_str());
}

double StringTokenizer::nextFloatToken()
{
   return _wtof(nextToken().c_str());
}

std::wstring StringTokenizer::nextToken(const std::wstring& delimiter)
{
   if (token_str.length() == 0)
     return L"";

   std::wstring  tmp_str = L"";
   unsigned int pos     = token_str.find(delimiter,0);

   if (pos != std::wstring::npos)
   {
      tmp_str   = token_str.substr(0,pos);
      token_str = token_str.substr(pos + delimiter.length(),token_str.length() - pos);
   }
   else
   {
      tmp_str   = token_str.substr(0,token_str.length());
      token_str = L"";
   }

   return tmp_str;
}

std::wstring StringTokenizer::remainingString()
{
   return token_str;
}

std::wstring StringTokenizer::filterNextToken(const std::wstring& filterStr)
{
   std::wstring  tmp_str    = nextToken();
   unsigned int currentPos = 0;

   while((currentPos = tmp_str.find(filterStr,currentPos)) != std::wstring::npos)
   {
      tmp_str.erase(currentPos,filterStr.length());
   }

   return tmp_str;
}
