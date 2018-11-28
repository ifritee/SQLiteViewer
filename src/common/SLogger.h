/*
 * Project SLogger
 * Copyright (C) 2017  Nikishin E.V.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SLOGGER_H
#define	SLOGGER_H

#include <iomanip>
#include <string>
#include <sstream>
#include <assert.h>

#if defined(WIN32)
#include <windows.h>
#elif defined(__unix__)
#include <pthread.h>
#endif  //defined (WIN32)

// Упростим себе жизнь
#ifndef slogger
#define slogger slog::SLogger::instance()
#endif

/**
 * @namespace slog
 * @brief Включает в себя элементы, позволяющие вести логи по ходу работы программы.
 *
 * Для нормальной работы вывода стека необходима библиотека boost > 1.65,
 * подключить библиотеку libdl.a и добавить дефайн STACKTRACE_USE
 */
namespace slog
{
  /** @brief Макрос подставляющий имя функции, имя файла и номер строки*/
  #define MORE_DATA(A) std::string(__FILE__).append(":").append(slog::SLogger::GetNormalLine_sstr(__LINE__)).append(" >> ").append(A)

  //----- Поправить если необходимо -----
  typedef unsigned int uint32_t;

  /** @brief Размер буфера для записи сообщений*/
  const unsigned int BufferSize_cui = 10000000;

  /** @brief Максимальный размер файла лога в байтах*/
  const unsigned int SLoggerMaxFileSize_cui = 2 * 1e7;

  /** @brief Показывает куда нужно записывать информацию по ходу работы программы*/
  enum WriteState_En {
    _InFile_en,       ///< @brief Записывать сразу в файл (самый медленный, но самый надежный способ сохранения лога)
    _InMemory_en,     ///< @brief Сначала записывать в RAM, а только затем в файл (Самый быстрый способ, но не надежный)
    _InOut_en,        ///< @brief Просто вывод в консоль
    _InFileAndOut_en  ///< @brief Вывод в консоль и в файл (самый медленный, но самый информативный
  };

  /** @brief Показывает тип посылаемого сообщеня*/
  enum MessageType_En{
    /** @brief Сообщение несет чисто информационный характер*/
    _Info_en = 0,
    /** @brief Предупреждение*/
    _Warning_en,
    /** Ошибка */
    _Error_en,
    /** Фатальная ошибка, приводящая к вылету из программы*/
    _Fatal_en
  };

  class SLogger;

  /**
   * @class SLoggerDestroyer
   * @brief Класс-деструктор для SLogger
   */
  class SLoggerDestroyer
  {
  private:
    SLogger * _SLogger_po;
  public:
    virtual ~SLoggerDestroyer();
    void InitDestoyer_v(SLogger* SLogger_po);
  };

  /**
   * @class SLogger
   * @brief Предназначен для записи текстовой информации в файл по ходу работы программы...
   */
  class SLogger
  {
  private:
    static SLogger * _SLogger_po; ///< @brief Объект класса, с которым идет работа
    static SLoggerDestroyer _SLoggerDestroyer_o; ///< @brief Объект класса-деструктора

  protected:
    /** @brief Конструктор */
    SLogger(std::string LogFilename_str = "ProgramLog.txt", WriteState_En WriteState_o = _InFile_en, MessageType_En MinOutputMsgLvl_en = _Info_en);
    /** @brief Деструктор */
    virtual ~SLogger();

    friend class SLoggerDestroyer;  ///< @brief Друг

  public:
    /** @brief Метод для получения объекта _APIInternalData_po */
    static SLogger* instance();

    /**
     * @brief Записываем информационную строку в выводной буфер
     * @param String Строка записываемая в буфер
     */
    SLogger& info(const char *String_pcc);
    SLogger& info(const std::string &String_ro);
    SLogger& info(const bool Flag_b);
    SLogger& info(const int Value_i);
    SLogger& info(const unsigned int Value_ui);
    SLogger& info(const float Value_f);

    SLogger& warning(const char *String_pcc);
    SLogger& warning(const std::string &String_ro);

    SLogger& error(const char *String_pcc);
    SLogger& error(const std::string &String_ro);

    SLogger& fatal(const char *String);
    SLogger& fatal(const std::string &String_ro);

    void SetAssert_v(bool);
    void SetAssert_v(bool, const char *String);
    void SetAssert_v(bool, const std::string &String_ro);

    /** @brief SetMinOutputMsgLvl_v Функция установки нового минимального выводимого уровня сообщений */
    void SetMinOutputMsgLvl_v(MessageType_En NewMinOutputMsgLvl_en);

    //----- СТАТИЧЕСКИЕ ФУНКЦИИ ------------------------------------------------
    template <class T>
    static std::string GetNormalLine_sstr(T Variable){std::stringstream ss; ss<<Variable; return ss.str();}

  private:
    /** @brief Имя файла в который будет сохраняться вся информация логгироваия*/
    std::string _LogFilename_str;
    /** @brief Запишем всю поступающую информацию в контейнер*/
    std::string _OutBuffer_str;
    /** @brief Показывает положение курсора в буфере*/
    uint32_t _WritePosition_ui32;
    /** @brief Покажем куда записывать информацию*/
    WriteState_En _WriteState_o;
    /** @brief Сохраним версию продукта*/
    std::string _Version_str;
    /** @brief Минимальное значение типа выводимой информации */
    MessageType_En _MinOutputMsgLvl_en;

#if defined(WIN32)
    CRITICAL_SECTION _Mutex_o;
#elif defined(__unix__)
    pthread_mutex_t _Mutex_o;
#endif  //WIN32

    //----- ЗАКРЫТЫЕ ФУНКЦИИ ---------------------------------------------------
    void Lock_v() {
#if defined(WIN32) && !defined(__MINGW32__)
      return; // EnterCriticalSection(&_Mutex_o); // Не работает в винде
#elif defined(__unix__)
      pthread_mutex_lock(&_Mutex_o);
#else
      return;
#endif  //defined (WIN32)
    }
    //---------------
    void UnLock_v() {
#if defined(WIN32) && !defined(__MINGW32__)
      return; // LeaveCriticalSection(&_Mutex_o); // Не работает в винде
#elif defined(__unix__)
      pthread_mutex_unlock(&_Mutex_o);
#else
      return;
#endif  //defined (WIN32)
    }
    /**
     * @brief Функция для записи строки в файл лога.
     * @param InfoString Сама строка, которую нужно записать
     * @param MessageType Тип предупреждения строки
     */
    void WriteString_v(const std::string & InfoString, MessageType_En MessageType);

    /**
     * @brief Функция возвращающая строку времени в формате: [ Число Месяц Год ::: Часы:Минуты:Секунды ]
     * @return Строку времени
     */
    std::string GetCurrentTime_str();

    /**
     * @brief Функция возвращающая строку с введенным состоянием
     * @return Строка состояния
     */
    std::string GetCurrentState_str(MessageType_En MessageType);

    /**
     * @brief Сохраняет переданную информацию в выбранной памяти
     * @param String строка для сохранения
     */
    void SaveInformation_v(const std::string &String);

    /**
     * @brief Сохраняем данные в файле.
     * @param String строка для сохранения
     */
    void SaveDataInFile_v(std::string String);

    /**
     * @brief Возвращает данные о файле - время создания CRC сумму и т. д.
     * @return Строка с данными
     */
    std::string GetBeginData_str();

    /**
     * @brief Возвращает имя ОС, в которой производился запуск ПО
     * @return Название ОС
     */
    std::string GetCurrentOS_str();

    /**
     * @brief Выдача стека вызовов
     * @return Форматированный текст стека вызовов
     */
    std::string TraceStack_str();
  };

  /** @brief Функция преобразования простых типов в std::string */
  template<class T>
  std::string ValueToString_str(T Value_t, int Precision_i = 4) {
    std::stringstream StringStream_o;
    StringStream_o<<std::fixed<<std::setprecision(Precision_i)<<Value_t;
    return StringStream_o.str();
  }

#ifndef __STRING
#define __STRING(x) #x
#endif
#define log_assert(expr) \
  slog::SLogger::instance()->SetAssert_v(expr, MORE_DATA(std::string("Assert условие не выполнено: ").append(__STRING(expr))))
}

#endif	/* SLOGGER_H */

