/*****************************************************************************
 * Alpine Terrain Renderer
 * Copyright (C) 2023 Adam Celarek
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
 *****************************************************************************/

#include "StdBuffer.h"

StdBuffer::StdBuffer(QObject* parent)
    : QObject{parent}
    , m_timer(this)
{
    std::cout.rdbuf(m_string_out.rdbuf());
    connect(&m_timer, &QTimer::timeout, this, &StdBuffer::process_str_out);
    m_timer.start(10);
    QTimer::singleShot(0, this, &StdBuffer::process_str_out);
}

const QString& StdBuffer::buffer() const
{
    return m_buffer;
}

void StdBuffer::set_buffer(const QString& new_buffer)
{
    if (m_buffer == new_buffer)
        return;
    m_buffer = new_buffer;
    emit buffer_changed(m_buffer);
}

void StdBuffer::append(const QString& string)
{
    process_str_out();
    set_buffer(buffer() + string);
}

void StdBuffer::append(const std::string& string)
{
    append(QString(string.c_str()));
}

void StdBuffer::process_str_out()
{
    set_buffer(buffer() + QString(m_string_out.str().c_str()));
    m_string_out.str("");
    m_string_out.clear();
}
