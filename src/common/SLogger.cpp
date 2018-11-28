/*
 * File:   SLogger.cpp
 * Author: ifritee
 *
 * Created on 19 Август 2013 г., 17:31
 */

#include <fstream>
#include <iostream>
#include <ctime>
#include <ios>

#ifdef __unix__
#include <unistd.h>
#include <sys/stat.h>
#elif WIN32
#include <windows.h>
#endif

#include "SLogger.h"

#ifdef STACKTRACE_USE
  #define BOOST_STACKTRACE_USE_ADDR2LINE
  #include <boost/version.hpp>
  #if BOOST_VERSION >= 106500
  #include <boost/stacktrace.hpp>
  #endif
#endif

namespace slog
{
  SLogger * SLogger::_SLogger_po = 0;

  SLoggerDestroyer SLogger::_SLoggerDestroyer_o;

  SLoggerDestroyer::~SLoggerDestroyer()
  {
    delete SLogger::_SLogger_po;
  }

  void SLoggerDestroyer::InitDestoyer_v(SLogger * SLogger_po)
  {
    SLogger::_SLogger_po = SLogger_po;
  }

  SLogger::SLogger(std::string LogFilename_str, WriteState_En WriteState_o, MessageType_En MinOutputMsgLvl_en):
    _MinOutputMsgLvl_en(MinOutputMsgLvl_en)
  {
#if defined(WIN32) && !defined(__MINGW32__)
  InitializeCriticalSection(&_Mutex_o);
#elif defined(__unix__)
  pthread_mutex_init(&_Mutex_o, NULL);
#endif  //WIN32
    _LogFilename_str = LogFilename_str;
    _OutBuffer_str.reserve(BufferSize_cui);
    _WriteState_o = WriteState_o;
#ifdef WIN32
    _WriteState_o = _InFile_en;
#endif
    _WritePosition_ui32 = 0;
    if(_WriteState_o == _InMemory_en || _WriteState_o == _InFile_en){
      std::ofstream File_o;
      File_o.open(_LogFilename_str.c_str(), std::ios::app);
      if(File_o.fail()) { // Если не смогли создать файл
#ifdef __unix__
        _LogFilename_str = std::string("/tmp/").append(_LogFilename_str);
        File_o.open(_LogFilename_str.c_str(), std::ios::app);
#endif
      }
      File_o.close();
      SaveInformation_v(GetBeginData_str());
    }
  }

  SLogger::~SLogger()
  {
    info("Success closing log file.");
    if(_WriteState_o == _InMemory_en){
      SaveDataInFile_v(_OutBuffer_str);
      _OutBuffer_str.clear();
    }
#if defined(WIN32)
  DeleteCriticalSection(&_Mutex_o);
#elif defined(__unix__)
  pthread_mutex_destroy(&_Mutex_o);
#endif  //WIN32
  }

  SLogger* SLogger::instance()
  {    
    if(!_SLogger_po){
      _SLogger_po = new SLogger();
      _SLoggerDestroyer_o.InitDestoyer_v(_SLogger_po);
    }
    return _SLogger_po;
  }

  SLogger& SLogger::info(const char* String_pcc)
  {
    WriteString_v(std::string(String_pcc), _Info_en);
    return *this;
  }

  SLogger& SLogger::info(const std::string &String_ro)
  {
    WriteString_v(String_ro, _Info_en);
    return *this;
  }

  SLogger& SLogger::info(const bool Flag_b)
  {
    WriteString_v(ValueToString_str(Flag_b), _Info_en);
    return *this;
  }

  SLogger& SLogger::info(const int Value_i)
  {
    WriteString_v(ValueToString_str(Value_i), _Info_en);
    return *this;
  }

  SLogger& SLogger::info(const unsigned int Value_ui)
  {
    WriteString_v(ValueToString_str(Value_ui), _Info_en);
    return *this;
  }

  SLogger& SLogger::info(const float Value_f)
  {
    WriteString_v(ValueToString_str(Value_f), _Info_en);
    return *this;
  }

  SLogger& SLogger::warning(const char* String_pcc)
  {
    WriteString_v(std::string(String_pcc), _Warning_en);
    return *this;
  }

    SLogger& SLogger::warning(const std::string &String_ro)
  {
    WriteString_v(String_ro, _Warning_en);
    return *this;
  }

  SLogger& SLogger::error(const char* String_pcc)
  {
    WriteString_v(std::string(String_pcc), _Error_en);
    return *this;
  }

  SLogger& SLogger::error(const std::string &String_ro)
  {
    WriteString_v(String_ro, _Error_en);
    return *this;
  }

  SLogger& SLogger::fatal(const char* String)
  {
    WriteString_v(std::string(String), _Fatal_en);
    return *this;
  }

  SLogger& SLogger::fatal(const std::string &String_ro)
  {
    WriteString_v(String_ro, _Fatal_en);
    return *this;
  }

  void SLogger::SetAssert_v(bool IsPass_b)
  {
    if(!IsPass_b) {
      info(TraceStack_str());
      fatal(MORE_DATA(std::string("Assert условие не выполнено: ")));
      assert(IsPass_b);
    }
  }

  void SLogger::SetAssert_v(bool IsPass_b, const char *String)
  {
    if(!IsPass_b) {
      info(TraceStack_str());
      fatal(MORE_DATA(String));
      assert(IsPass_b);
    }
  }

  void SLogger::SetAssert_v(bool IsPass_b, const std::string &String_ro)
  {
    if(!IsPass_b) {
      info(TraceStack_str());
      fatal(MORE_DATA(String_ro));
      assert(IsPass_b);
    }
  }

  void SLogger::SetMinOutputMsgLvl_v(MessageType_En NewMinOutputMsgLvl_en)
  {
    _MinOutputMsgLvl_en = NewMinOutputMsgLvl_en;
  }

  //----- ЗАКРЫТЫЕ ФУНКЦИИ -----------------------------------------------------
    void SLogger::WriteString_v(const std::string & InfoString, MessageType_En MessageType)
  {
    if(_MinOutputMsgLvl_en > MessageType) {
      return;
    }
    std::string CurrentString_o = GetCurrentTime_str();
    CurrentString_o.append(GetCurrentState_str(MessageType));
    CurrentString_o.append(InfoString);
    SaveInformation_v(CurrentString_o);
  }

  std::string SLogger::GetCurrentTime_str()
  {
    char Buffer_o[80];
    time_t Seconds_o = time(NULL);
    tm* Timeinfo_po = localtime(&Seconds_o);
    strftime(Buffer_o, 80, "[%d.%m.%Y %H:%M:%S] ", Timeinfo_po);
    return std::string(Buffer_o);
  }

  std::string SLogger::GetCurrentState_str(MessageType_En MessageType)
  {
    std::string MessageType_str;
    switch(MessageType){
    case _Info_en:
      MessageType_str.append("(II) ");
      break;
    case _Warning_en:
      MessageType_str.append("(WW) ");
      break;
    case _Error_en:
      MessageType_str.append("(EE) ");
      break;
    case _Fatal_en:
      MessageType_str.append("(FF) ");
      break;
    }
    return MessageType_str;
  }

  void SLogger::SaveInformation_v(const std::string & String)
  {
    switch(_WriteState_o) {
    case _InOut_en:{
      std::cout<<String<<std::endl;
    } break;
    case _InMemory_en: {
      //----- Проверка на выход за границы массива
      this->Lock_v();
      if(_OutBuffer_str.size() + String.size() >= BufferSize_cui) {
#ifndef __NO_SLogger__
        std::cout<<String<<std::endl;
        SaveDataInFile_v(_OutBuffer_str);
#endif // __NO_SLogger__
        _OutBuffer_str.clear();
      }
      _OutBuffer_str.append(String);
      _OutBuffer_str.append("\n");
      this->UnLock_v();
    } break;
    case _InFile_en:{
      this->Lock_v();
#ifndef __NO_SLogger__
      SaveDataInFile_v(String);
#endif // __NO_SLogger__
      this->UnLock_v();
    } break;
    case _InFileAndOut_en:{
      this->Lock_v();
#ifndef __NO_SLogger__
      std::cout<<String<<std::endl;
      SaveDataInFile_v(String);
#endif // __NO_SLogger__
      this->UnLock_v();
    } break;
    }
  }

  void SLogger::SaveDataInFile_v(std::string String)
  {
    std::ofstream File_o;
    File_o.open(_LogFilename_str.c_str(), std::ios::app);
    if(File_o.tellp() >= SLoggerMaxFileSize_cui) {
      File_o.close();
      File_o.open(_LogFilename_str.c_str(), std::ios::trunc);
      File_o<<GetBeginData_str()<<"\n";
    }
    File_o<<String.c_str()<<std::endl;
    File_o.close();
  }

  std::string SLogger::GetBeginData_str()
  {
#if defined(__unix__) || defined(__gnu_linux__)
    char FullBinaryName_ar[256];
    FullBinaryName_ar[readlink("/proc/self/exe", FullBinaryName_ar, sizeof(FullBinaryName_ar))] = '\0';
    struct stat BinaryParameters_o;
    stat(FullBinaryName_ar, &BinaryParameters_o);
    //----- Заполняем строку -----
    std::string String("\n\n\n\n\nName: ");
    String.append(FullBinaryName_ar).append("\n");
    String.append("Version: ").append(_Version_str).append("\n");
    tm* Timeinfo_po = localtime(&BinaryParameters_o.st_mtim.tv_sec);
    strftime(FullBinaryName_ar, sizeof(FullBinaryName_ar), "%d.%m.%Y %H:%M:%S ", Timeinfo_po);
    String.append("Build on: ").append(FullBinaryName_ar).append("\n");
    String.append("Based on: ").append(__VERSION__).append("\n\n");
    String.append("Markers: (II) informational, (WW) warning, (EE) error, (FF) fatal error.\n");
    String.append("Current Operating System: ").append(GetCurrentOS_str()).append("\n");
    String.append("Runned at: ").append(GetCurrentTime_str()).append("\n");
    return String;
#else
    //----- Заполняем строку -----
    char FullBinaryName_ar[256];
    GetModuleFileNameA(NULL, FullBinaryName_ar, sizeof(FullBinaryName_ar));
    std::string String("Name: ");
    String.append(FullBinaryName_ar).append("\n");
    String.append("Version: ").append(_Version_str).append("\n");
    SYSTEMTIME SystemTime_o;
    GetLocalTime(&SystemTime_o);
    wsprintfA(FullBinaryName_ar, "%d-%02d-%02d", SystemTime_o.wYear, SystemTime_o.wMonth, SystemTime_o.wDay);
    String.append("Build on: ").append(FullBinaryName_ar).append("\n");
    String.append("Based on: ").append(__VERSION__).append("\n\n");
    String.append("Markers: (II) informational, (WW) warning, (EE) error, (FF) fatal error.\n");
    String.append("Current Operating System: ").append(GetCurrentOS_str()).append("\n");
    String.append("Runned at: ").append(GetCurrentTime_str()).append("\n");
    return String;
#endif
  }

  std::string SLogger::GetCurrentOS_str()
  {
    std::string String;
#ifdef __gnu_linux__
    String = "Linux";
#elif __unix__
    String = "Unix";
#elif WIN32
    String = "Windows";
#endif
    return String;
  }
  std::string SLogger::TraceStack_str()
  {
    std::stringstream OutStream_o;
#ifdef STACKTRACE_USE
#if BOOST_VERSION >= 106500
    OutStream_o << boost::stacktrace::stacktrace();
#endif  //BOOST_VERSION >= 106500
#endif // STACKTRACE_USE
    return OutStream_o.str();
  }

}
