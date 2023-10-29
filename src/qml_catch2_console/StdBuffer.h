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

#pragma once

#include <QObject>
#include <QTimer>
#include <iostream>
#include <sstream>

class StdBuffer : public QObject
{
    Q_OBJECT
public:
    explicit StdBuffer(QObject* parent = nullptr);

    [[nodiscard]] const QString& buffer() const;
    void set_buffer(const QString& new_buffer);
    void append(const QString& string);
    void append(const std::string& string);
    void process_str_out();

signals:

    void buffer_changed(const QString& buffer);

private:
    QString m_buffer;
    std::ostringstream m_string_out;
    QTimer m_timer;
    Q_PROPERTY(QString buffer READ buffer WRITE set_buffer NOTIFY buffer_changed FINAL)
};
