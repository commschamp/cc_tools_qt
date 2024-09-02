//
// Copyright 2023 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "SslClientSocketConfigWidget.h"

#include <vector>
#include <utility>

#include <QtGlobal>
#include <QtWidgets/QFileDialog>

#include "comms/CompileControl.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace 
{

using PeerVerifyModeMap = std::vector<std::pair<QString, QSslSocket::PeerVerifyMode>>;
using SslProtocolMap = std::vector<std::pair<QString, QSsl::SslProtocol>>;
using EncodingFormatMap = std::vector<std::pair<QString, QSsl::EncodingFormat>>;
using KeyAlgorithmMap = std::vector<std::pair<QString, QSsl::KeyAlgorithm>>;

template <typename TMap>
void fillComboBox(const TMap& map, QComboBox& box)
{
    for (auto& elem : map) {
        box.addItem(elem.first, static_cast<int>(elem.second));
    }    
}

template <typename TMap, typename TVal>
void selectComboBoxText(const TMap& map, TVal value, QComboBox& box)
{
    for (auto& elem : map) {
        if (elem.second == value) {
            box.setCurrentText(elem.first);
            return;
        }
    }

    assert(false); // Should not happen
    box.setCurrentText(map.front().first);
}

const PeerVerifyModeMap& peerVerifyModeMap()
{
    static const PeerVerifyModeMap Map = {
        {"No", QSslSocket::VerifyNone},
        {"Yes", QSslSocket::VerifyPeer}
    };

    return Map;
}

COMMS_MSVC_WARNING_PUSH
COMMS_MSVC_WARNING_DISABLE(4996) // Disable deprecated values warning
const SslProtocolMap& sslProtocolMap()
{
    static const SslProtocolMap Map = {
        {"Any Secure", QSsl::SecureProtocols},
        {"Any Supported", QSsl::AnyProtocol},
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)        
        {"TLSv1.0", QSsl::TlsV1_0},
        {"TLSv1.0+", QSsl::TlsV1_0OrLater},
        {"TLSv1.1", QSsl::TlsV1_1},
        {"TLSv1.1+", QSsl::TlsV1_1OrLater},
        {"TLSv1.2", QSsl::TlsV1_2},
#endif // #if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)         
        {"TLSv1.2+", QSsl::TlsV1_2OrLater},
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        {"TLSv1.3", QSsl::TlsV1_3},
        {"TLSv1.3+", QSsl::TlsV1_3OrLater},
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        {"DTLSv1.0", QSsl::DtlsV1_0},
        {"DTLSv1.0+", QSsl::DtlsV1_0OrLater},
        {"DTLSv1.2", QSsl::DtlsV1_2},
        {"DTLSv1.2+", QSsl::DtlsV1_2OrLater},
    };

    return Map;
}
COMMS_MSVC_WARNING_POP

const EncodingFormatMap& encodingFormatMap()
{
    static const EncodingFormatMap Map = {
        {"PEM", QSsl::Pem},
        {"DER", QSsl::Der}
    };

    return Map;
}

const KeyAlgorithmMap& keyAlgorithmMap()
{
    static const KeyAlgorithmMap Map = {
        {"RSA", QSsl::Rsa},
        {"DSA", QSsl::Dsa},
        {"EC", QSsl::Ec},
        {"DH", QSsl::Dh},
    };

    return Map;
}

} // namespace 
    

SslClientSocketConfigWidget::SslClientSocketConfigWidget(
    SslClientSocket& socket,
    QWidget* parentObj)
  : Base(parentObj),
    m_socket(socket)
{
    m_ui.setupUi(this);

    m_ui.m_portSpinBox->setRange(
        1,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    fillComboBox(encodingFormatMap(), *m_ui.m_caDirFormatComboBox);
    fillComboBox(encodingFormatMap(), *m_ui.m_caFileFormatComboBox);
    fillComboBox(peerVerifyModeMap(), *m_ui.m_verifyHostComboBox);
    fillComboBox(sslProtocolMap(), *m_ui.m_protocolComboBox);    
    fillComboBox(encodingFormatMap(), *m_ui.m_certFormatComboBox);
    fillComboBox(keyAlgorithmMap(), *m_ui.m_privKeyAlgComboBox);
    fillComboBox(encodingFormatMap(), *m_ui.m_privKeyFormatComboBox);

    refresh();

    connect(
        m_ui.m_hostLineEdit,  &QLineEdit::textChanged,
        this, &SslClientSocketConfigWidget::hostValueChanged);

    connect(
        m_ui.m_portSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(portValueChanged(int)));

    connect(
        m_ui.m_caDirLineEdit, &QLineEdit::textChanged,
        this, &SslClientSocketConfigWidget::caDirValueChanged);  

    connect(
        m_ui.m_caDirToolButton, SIGNAL(clicked()),
        this, SLOT(caDirSelectClicked()));

    connect(
        m_ui.m_caDirFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(caDirFormatIndexChanged(int)));     

    connect(
        m_ui.m_caFileLineEdit, &QLineEdit::textChanged,
        this, &SslClientSocketConfigWidget::caFileValueChanged);  

    connect(
        m_ui.m_caFileToolButton, SIGNAL(clicked()),
        this, SLOT(caFileSelectClicked()));

    connect(
        m_ui.m_caFileFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(caFileFormatIndexChanged(int)));         

    connect(
        m_ui.m_verifyHostComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(verifyHostIndexChanged(int)));     

    connect(
        m_ui.m_verifyNameLineEdit, &QLineEdit::textChanged,
        this, &SslClientSocketConfigWidget::verifyNameValueChanged);          

    connect(
        m_ui.m_protocolComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(sslProtocolIndexChanged(int)));                   

    connect(
        m_ui.m_certLineEdit,  &QLineEdit::textChanged,
        this, &SslClientSocketConfigWidget::certValueChanged);

    connect(
        m_ui.m_certToolButton, SIGNAL(clicked()),
        this, SLOT(certSelectClicked()));         

    connect(
        m_ui.m_certFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(certFormatIndexChanged(int)));     

    connect(
        m_ui.m_privKeyLineEdit,  &QLineEdit::textChanged,
        this, &SslClientSocketConfigWidget::privKeyValueChanged);

    connect(
        m_ui.m_privKeyToolButton, SIGNAL(clicked()),
        this, SLOT(privKeySelectClicked()));        

    connect(
        m_ui.m_privKeyAlgComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(privKeyAlgIndexChanged(int)));             

    connect(
        m_ui.m_privKeyFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(privKeyFormatIndexChanged(int)));

    connect(
        m_ui.m_privKeyPassLineEdit,  &QLineEdit::textChanged,
        this, &SslClientSocketConfigWidget::privKeyPassValueChanged);

    connect(
        m_ui.m_privKeyShowHidePushButton,  &QPushButton::clicked,
        this, &SslClientSocketConfigWidget::privKeyShowHideClicked);        
}

SslClientSocketConfigWidget::~SslClientSocketConfigWidget() noexcept = default;

void SslClientSocketConfigWidget::refresh()
{
    m_ui.m_hostLineEdit->setText(m_socket.getHost());
    m_ui.m_portSpinBox->setValue(static_cast<int>(m_socket.getPort()));
    m_ui.m_caDirLineEdit->setText(m_socket.getCaDir());
    selectComboBoxText(encodingFormatMap(), m_socket.getCaDirFormat(), *m_ui.m_caDirFormatComboBox);     
    m_ui.m_caFileLineEdit->setText(m_socket.getCaFile());
    selectComboBoxText(encodingFormatMap(), m_socket.getCaFileFormat(), *m_ui.m_caFileFormatComboBox);     
    selectComboBoxText(peerVerifyModeMap(), m_socket.getVerifyMode(), *m_ui.m_verifyHostComboBox);
    m_ui.m_verifyNameLineEdit->setText(m_socket.getVerifyName());
    selectComboBoxText(sslProtocolMap(), m_socket.getSslProtocol(), *m_ui.m_protocolComboBox);
    m_ui.m_certLineEdit->setText(m_socket.getCertFile());
    selectComboBoxText(encodingFormatMap(), m_socket.getCertFormat(), *m_ui.m_certFormatComboBox);     
    m_ui.m_privKeyLineEdit->setText(m_socket.getPrivKeyFile());
    selectComboBoxText(keyAlgorithmMap(), m_socket.getPrivKeyAlg(), *m_ui.m_privKeyAlgComboBox);     
    selectComboBoxText(encodingFormatMap(), m_socket.getPrivKeyFormat(), *m_ui.m_privKeyFormatComboBox);     
    m_ui.m_privKeyPassLineEdit->setText(m_socket.getPrivKeyPass());
}

void SslClientSocketConfigWidget::hostValueChanged(const QString& value)
{
    m_socket.setHost(value);
}

void SslClientSocketConfigWidget::portValueChanged(int value)
{
    m_socket.setPort(static_cast<PortType>(value));
}

void SslClientSocketConfigWidget::caDirValueChanged(const QString& value)
{
    m_socket.setCaDir(value);
}

void SslClientSocketConfigWidget::caDirSelectClicked()
{
    QString dir = 
        QFileDialog::getExistingDirectory(
            this, 
            tr("Select CA Directory"),
            m_socket.getCaDir(),
            QFileDialog::ShowDirsOnly);

    if (!dir.isEmpty()) {
        m_ui.m_caDirLineEdit->setText(dir);
    }
}

void SslClientSocketConfigWidget::caDirFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setCaDirFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_caDirFormatComboBox->currentData().toInt()));
}

void SslClientSocketConfigWidget::caFileValueChanged(const QString& value)
{
    m_socket.setCaFile(value);
}

void SslClientSocketConfigWidget::caFileSelectClicked()
{
    QString file = 
        QFileDialog::getOpenFileName(
            this, 
            tr("Select CA File"),
            m_socket.getCaFile());

    if (!file.isEmpty()) {
        m_ui.m_caFileLineEdit->setText(file);
    }
}

void SslClientSocketConfigWidget::caFileFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setCaFileFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_caFileFormatComboBox->currentData().toInt()));
}

void SslClientSocketConfigWidget::verifyHostIndexChanged([[maybe_unused]] int value)
{
    m_socket.setVerifyMode(static_cast<QSslSocket::PeerVerifyMode>(m_ui.m_verifyHostComboBox->currentData().toInt()));
}

void SslClientSocketConfigWidget::verifyNameValueChanged(const QString& value)
{
    m_socket.setVerifyName(value);
}

void SslClientSocketConfigWidget::sslProtocolIndexChanged([[maybe_unused]] int value)
{
    m_socket.setSslProtocol(static_cast<QSsl::SslProtocol>(m_ui.m_protocolComboBox->currentData().toInt()));
}

void SslClientSocketConfigWidget::certValueChanged(const QString& value)
{
    m_socket.setCertFile(value);
}

void SslClientSocketConfigWidget::certSelectClicked()
{
    QString file = 
        QFileDialog::getOpenFileName(
            this, 
            tr("Select Local Certificate File"),
            m_socket.getCertFile());

    if (!file.isEmpty()) {
        m_ui.m_certLineEdit->setText(file);
    }
}

void SslClientSocketConfigWidget::certFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setCertFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_certFormatComboBox->currentData().toInt()));
}

void SslClientSocketConfigWidget::privKeyValueChanged(const QString& value)
{
    m_socket.setPrivKeyFile(value);
}

void SslClientSocketConfigWidget::privKeySelectClicked()
{
    QString file = 
        QFileDialog::getOpenFileName(
            this, 
            tr("Select Private Key File"),
            m_socket.getPrivKeyFile());

    if (!file.isEmpty()) {
        m_ui.m_privKeyLineEdit->setText(file);
    }
}

void SslClientSocketConfigWidget::privKeyAlgIndexChanged([[maybe_unused]] int value)
{
    m_socket.setPrivKeyAlg(static_cast<QSsl::KeyAlgorithm>(m_ui.m_privKeyAlgComboBox->currentData().toInt()));
}

void SslClientSocketConfigWidget::privKeyFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setPrivKeyFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_privKeyFormatComboBox->currentData().toInt()));
}

void SslClientSocketConfigWidget::privKeyPassValueChanged(const QString& value)
{
    m_socket.setPrivKeyPass(value);
}

void SslClientSocketConfigWidget::privKeyShowHideClicked(bool checked)
{
    auto mode = QLineEdit::Password;
    auto buttonText = tr("Show");
    if (checked) {
        mode = QLineEdit::Normal;
        buttonText = tr("Hide");
    }
    
    m_ui.m_privKeyPassLineEdit->setEchoMode(mode);
    m_ui.m_privKeyShowHidePushButton->setText(buttonText);
}

}  // namespace plugin

}  // namespace cc_tools_qt


