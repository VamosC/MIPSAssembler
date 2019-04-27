#ifndef HIGNLIGHTER_H
#define HIGNLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit Highlighter(QTextDocument *parent=nullptr);
protected:
    void highlightBlock(const QString &text) override;//重写父类这个函数 自动调用
private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat labelFormat;
    QTextCharFormat constantFormat;
    QTextCharFormat registerFormat;
};

#endif // HIGNLIGHTER_H
