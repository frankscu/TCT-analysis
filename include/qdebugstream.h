/**
 * \file
 * \brief Definition of the QDebugStream class.
 * \details Responsible for output redirect to the log file and console.
 */

#ifndef Q_DEBUG_STREAM_H
#define Q_DEBUG_STREAM_H

#include <iostream>
#include <streambuf>
#include <string>
#include <fstream>

#include "qtextedit.h"

class QDebugStream : public std::basic_streambuf<char>
{
public:
 QDebugStream(std::ostream &stream, std::ofstream &log_stream, QTextEdit* text_edit) : m_stream(stream), log_file(log_stream)
 {
  log_window = text_edit;
  m_old_buf = stream.rdbuf();
  stream.rdbuf(this);
 }
 ~QDebugStream()
 {
  // output anything that is left
  if (!m_string.empty()) {
   log_window->append(m_string.c_str());
   log_file<<m_string.c_str();
  }

  m_stream.rdbuf(m_old_buf);
 }

protected:
 virtual int_type overflow(int_type v)
 {
  if (v == '\n')
  {
   log_window->append(m_string.c_str());
   log_file<<m_string.c_str()<<"\n";
   m_string.erase(m_string.begin(), m_string.end());
  }
  else
   m_string += v;

  return v;
 }

 virtual std::streamsize xsputn(const char *p, std::streamsize n)
 {
  m_string.append(p, p + n);

  int pos = 0;
  while (pos != std::string::npos)
  {
   pos = m_string.find('\n');
   if (pos != std::string::npos)
   {
    std::string tmp(m_string.begin(), m_string.begin() + pos);
    log_file<<tmp.c_str();
    log_window->append(tmp.c_str());
    QCoreApplication::processEvents();
    m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
   }
  }

  return n;
 }

private:
 std::ostream &m_stream;
 std::ofstream &log_file;
 std::streambuf *m_old_buf;
 std::string m_string;
 QTextEdit* log_window;
};

#endif
