#include "authwindow.h"
#include "iostream"
#include "mainwindow.h"
#include <QMessageBox>

AuthWindow::AuthWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Админ-авторизация");
    setMinimumSize(300, 200);

    auto *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Вход администратора");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Логин");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Пароль");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    loginButton = new QPushButton("Войти", this);
    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::handleLogin);
    layout->addWidget(loginButton);

    registerButton = new QPushButton("Зарегистрироваться", this);
    connect(registerButton, &QPushButton::clicked, this, &AuthWindow::handleRegister);
    layout->addWidget(registerButton);



    setLayout(layout);

    QString style = R"(
        QWidget {
            background-color: #f8f8f8;
            color: #333;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            font-size: 14px;
        }
        QLabel {
            font-size: 16px;
            font-weight: bold;
            color: #007AFF;
        }
        QLineEdit {
            border: 1px solid #d0d0d0;
            border-radius: 8px;
            padding: 6px 10px;
            background-color: #ffffff;
            color: #333;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #007AFF;
            outline: none;
        }
        QPushButton {
            background-color: #007AFF;
            border: none;
            border-radius: 8px;
            color: #ffffff;
            font-size: 14px;
            padding: 8px 12px;
        }
        QPushButton:hover {
            background-color: #005ecb;
        }
        QPushButton:pressed {
            background-color: #004ba0;
        }
    )";
    setStyleSheet(style);
}

bool AuthWindow::checkCredentials(const QString &username, const QString &password) {
    QFile file("users.csv");
    qDebug() << "Файл будет создан здесь: " << QFileInfo(file).absoluteFilePath();
    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
            return false;
        }
        file.close();
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для чтения");
        return false;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 2) {
            QString storedUsername = parts[0].trimmed();
            QString storedPassword = parts[1].trimmed();
            if (storedUsername == username) {
                if (storedPassword == password) {
                    return true;

                }
            }
        }
    }

    return false;
}

void AuthWindow::handleLogin() {
    if (usernameInput->text().isEmpty() || passwordInput->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле ввода не заполнено");
        return;
    }

    QString username = usernameInput->text();
    QString password = usernameInput->text();

    if (checkCredentials(username, password)) {
        auto *mainWindow = new MainWindow();
        mainWindow->show();
        hide();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}

void AuthWindow::handleRegister() {
    if (usernameInput->text().isEmpty() || passwordInput->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле ввода не заполнено");
        return;
    }

    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (checkCredentials(username,password)) {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким логином уже существует");
        return;
    }

    QFile file("users.csv");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи");
        return;
    }

    QTextStream out(&file);
    out << username << "," << password << "\n";
    file.close();

    QMessageBox::information(this, "Успех", "Регистрация завершена!");
}

