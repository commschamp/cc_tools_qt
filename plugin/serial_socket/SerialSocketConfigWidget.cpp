//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "SerialSocketConfigWidget.h"

#include <cassert>
#include <type_traits>
#include <algorithm>
#include <iterator>

#include <QtSerialPort/QSerialPortInfo>

namespace cc_tools_qt
{

namespace plugin
{

namespace serial_socket
{

namespace
{

const QSerialPort::Parity ParityMap[] = {
    QSerialPort::NoParity,
    QSerialPort::EvenParity,
    QSerialPort::OddParity,
    QSerialPort::SpaceParity,
    QSerialPort::MarkParity
};

const int NumOfParityValues =
    std::extent<decltype(ParityMap)>::value;

const QSerialPort::StopBits StopBitMap[] = {
    QSerialPort::OneStop,
    QSerialPort::OneAndHalfStop,
    QSerialPort::TwoStop
};

const int NumOfStopBitValues =
    std::extent<decltype(StopBitMap)>::value;

QSerialPort::Parity mapParityFromIdx(int idx)
{
    if ((idx < 0) || (NumOfParityValues <= idx)) {
        [[maybe_unused]] static constexpr bool Invalid_index = false;
        assert(Invalid_index); 
        return ParityMap[0];
    }

    return ParityMap[idx];
}

int mapParityToIdx(QSerialPort::Parity value)
{
    auto iter = std::find(std::begin(ParityMap), std::end(ParityMap), value);
    if (iter == std::end(ParityMap)) {
        [[maybe_unused]] static constexpr bool Invalid_value = false;
        assert(Invalid_value); 
        return 0;
    }

    return static_cast<int>(std::distance(std::begin(ParityMap), iter));
}

QSerialPort::StopBits mapStopBitsFromIdx(int idx)
{
    if ((idx < 0) || (NumOfStopBitValues <= idx)) {
        [[maybe_unused]] static constexpr bool Invalid_index = false;
        assert(Invalid_index); 
        return StopBitMap[0];
    }

    return StopBitMap[idx];
}

int mapStopBitToIdx(QSerialPort::StopBits value)
{
    auto iter = std::find(std::begin(StopBitMap), std::end(StopBitMap), value);
    if (iter == std::end(StopBitMap)) {
        [[maybe_unused]] static constexpr bool Invalid_value = false;
        assert(Invalid_value); 
        return 0;
    }

    return static_cast<int>(std::distance(std::begin(StopBitMap), iter));
}

QSerialPort::FlowControl mapFlowControlFromIdx(int idx)
{
    return static_cast<QSerialPort::FlowControl>(idx);
}

int mapFlowControlToIdx(QSerialPort::FlowControl value)
{
    return static_cast<int>(value);
}

void populateAvailableSerialPorts(QComboBox& box)
{
    auto infos = QSerialPortInfo::availablePorts();
    QStringList devices;
    for (auto& i : infos) {
        devices.append(i.systemLocation());
    }

    devices.sort();
    box.clear();
    for (auto& d : devices) {
        box.addItem(d);
    }
}

}  // namespace

SerialSocketConfigWidget::SerialSocketConfigWidget(
    SerialSocket& socket,
    QWidget* parentObj)
  : Base(parentObj),
    m_socket(socket)
{
    m_ui.setupUi(this);
    populateAvailableSerialPorts(*m_ui.m_deviceComboBox);

    m_ui.m_deviceLineEdit->setText(m_socket.name());
    m_ui.m_baudSpinBox->setValue(m_socket.baud());
    m_ui.m_dataBitsSpinBox->setValue(m_socket.dataBits());
    m_ui.m_parityComboBox->setCurrentIndex(mapParityToIdx(m_socket.parity()));
    m_ui.m_stopBitsComboBox->setCurrentIndex(mapStopBitToIdx(m_socket.stopBits()));
    m_ui.m_flowComboBox->setCurrentIndex(mapFlowControlToIdx(m_socket.flowControl()));

    refreshDeviceConfig();

    connect(
        m_ui.m_deviceLineEdit, &QLineEdit::textEdited,
        this, &SerialSocketConfigWidget::nameChanged);

    connect(
        m_ui.m_deviceComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &SerialSocketConfigWidget::deviceChanged);         

    connect(
        m_ui.m_deviceRefreshToolButton, &QToolButton::clicked,
        this, &SerialSocketConfigWidget::deviceRefreshClicked);     
        
    connect(
        m_ui.m_deviceEditToolButton, &QToolButton::toggled,
        this, &SerialSocketConfigWidget::deviceEditClicked);          

    connect(
        m_ui.m_parityComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &SerialSocketConfigWidget::parityChanged);        

    connect(
        m_ui.m_baudSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &SerialSocketConfigWidget::baudChanged);

    connect(
        m_ui.m_dataBitsSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &SerialSocketConfigWidget::dataBitsChanged);

    connect(
        m_ui.m_parityComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &SerialSocketConfigWidget::parityChanged);

    connect(
        m_ui.m_stopBitsComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &SerialSocketConfigWidget::stopBitsChanged);

    connect(
        m_ui.m_flowComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
        this, &SerialSocketConfigWidget::flowControlChanged);
}

SerialSocketConfigWidget::~SerialSocketConfigWidget() noexcept = default;

void SerialSocketConfigWidget::nameChanged(const QString& value)
{
    if (m_editMode) {
        m_socket.name() = value;
        refreshDeviceConfig();
    }
}

void SerialSocketConfigWidget::deviceChanged(int idx)
{
    if (m_editMode) {
        return;
    }

    auto value = m_ui.m_deviceComboBox->itemText(idx);
    m_socket.name() = value;
    m_ui.m_deviceLineEdit->setText(value);
}

void SerialSocketConfigWidget::deviceRefreshClicked([[maybe_unused]] bool checked)
{
    m_ui.m_deviceComboBox->blockSignals(true);
    populateAvailableSerialPorts(*m_ui.m_deviceComboBox);
    refreshDeviceConfigComboSignalsBlocked();
    m_ui.m_deviceComboBox->blockSignals(false);
}

void SerialSocketConfigWidget::deviceEditClicked(bool checked)
{
    m_deviceEditForced = checked;
    refreshDeviceConfig();
}

void SerialSocketConfigWidget::baudChanged(int value)
{
    m_socket.baud() = static_cast<SerialSocket::Baud>(value);
}

void SerialSocketConfigWidget::dataBitsChanged(int value)
{
    m_socket.dataBits() = static_cast<SerialSocket::DataBits>(value);
}

void SerialSocketConfigWidget::parityChanged(int value)
{
    m_socket.parity() = mapParityFromIdx(value);
}

void SerialSocketConfigWidget::stopBitsChanged(int value)
{
    m_socket.stopBits() = mapStopBitsFromIdx(value);
}

void SerialSocketConfigWidget::flowControlChanged(int value)
{
    m_socket.flowControl() = mapFlowControlFromIdx(value);
}

void SerialSocketConfigWidget::refreshDeviceConfig()
{
    m_ui.m_deviceComboBox->blockSignals(true);
    refreshDeviceConfigComboSignalsBlocked();
    m_ui.m_deviceComboBox->blockSignals(false);
}

void SerialSocketConfigWidget::refreshDeviceConfigComboSignalsBlocked()
{
    bool found = false;
    for (auto idx = 0; idx < m_ui.m_deviceComboBox->count(); ++idx) {
        if (m_socket.name() == m_ui.m_deviceComboBox->itemText(idx)) {
            found = true;
            m_ui.m_deviceComboBox->setCurrentIndex(idx);
            break;
        }
    }

    if (!found) {
        m_ui.m_deviceComboBox->setCurrentIndex(-1);
        m_ui.m_deviceEditToolButton->setChecked(true);        
        m_deviceEditForced = true;
    }    

    m_ui.m_deviceEditToolButton->setEnabled(found);
    m_editMode = m_deviceEditForced || (!found);

    m_ui.m_deviceLineEdit->setVisible(m_editMode);
    m_ui.m_deviceComboBox->setHidden(m_editMode);
    m_ui.m_deviceRefreshToolButton->setHidden(m_editMode);
}

}  // namespace serial_socket

}  // namespace plugin

}  // namespace cc_tools_qt


