/*
 * chateditproxy.cpp - abstraction to change ChatEdit type in runtime
 * Copyright (C) 2007  Michail Pishchagin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "chateditproxy.h"

#include "msgmle.h"
#include "psioptions.h"

#include <QVBoxLayout>

static const QLatin1String expandingLineEdit("options.ui.chat.use-expanding-line-edit");
static const QLatin1String audioMessage("options.media.audio-message");

ChatEditProxy::ChatEditProxy(QWidget *parent) :
    QWidget(parent), lineEditEnabled_(false), textEdit_(nullptr), layout_(nullptr)
{
    layout_ = new QVBoxLayout(this);
    layout_->setMargin(0);
    layout_->setSpacing(0);

    connect(PsiOptions::instance(), &PsiOptions::optionChanged, this, [this](const QString &option) {
        if (option == expandingLineEdit)
            optionsChanged();
        if (option == audioMessage)
            addRrecordButton();
    });
    optionsChanged();

    if (!textEdit_)
        updateLayout();
}

/**
 * Creates new QTextEdit basing on ChatEditProxy's properties.
 */
ChatEdit *ChatEditProxy::createTextEdit()
{
    if (lineEditEnabled())
        return new LineEdit(this);
    return new ChatEdit(this);
}

/**
 * Moves the QTextDocument and QTextCursor data from \a oldTextEdit
 * to \a newTextEdit.
 *
 * NB: Make sure that all QSyntaxHighlighters are detached prior to calling
 * this function.
 */
void ChatEditProxy::moveData(QTextEdit *newTextEdit, QTextEdit *oldTextEdit) const
{
    QTextDocument *doc    = oldTextEdit->document();
    QTextCursor    cursor = oldTextEdit->textCursor();

    doc->setParent(newTextEdit);
    oldTextEdit->setDocument(nullptr);

    newTextEdit->setDocument(doc);
    newTextEdit->setTextCursor(cursor);
}

/**
 * Creates new QTextEdit and moves data to it from the old one.
 * Text, selection and cursor position are left intact.
 */
void ChatEditProxy::updateLayout()
{
    ChatEdit *newEdit = createTextEdit();

    if (textEdit_) {
        // all syntaxhighlighters should be removed while we move
        // the documents around, and should be reattached afterwards
        textEdit_->setCheckSpelling(false);
        newEdit->setCheckSpelling(false);

        moveData(newEdit, textEdit_);

        newEdit->setCheckSpelling(ChatEdit::checkSpellingGloballyEnabled());
    }

    delete textEdit_;
    textEdit_ = newEdit;
    layout_->addWidget(textEdit_);
    // Add sound record button if allowed and not exists
    addRrecordButton();
    emit textEditCreated(textEdit_);
}

/**
 * Update ChatEdit widget according to current options.
 */
void ChatEditProxy::optionsChanged()
{
    bool isLineEdit = PsiOptions::instance()->getOption(expandingLineEdit).toBool();
    if (lineEditEnabled_ != isLineEdit) {
        lineEditEnabled_ = isLineEdit;
        updateLayout();
    }
}

void ChatEditProxy::addRrecordButton()
{
    bool isEnabled = PsiOptions::instance()->getOption(audioMessage).toBool();
    if (!textEdit_->hasSoundRecButton() && isEnabled) {
        textEdit_->addSoundRecButton();
    } else if (textEdit_->hasSoundRecButton() && !isEnabled) {
        textEdit_->removeSoundRecButton();
    }
}
