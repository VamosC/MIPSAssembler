#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter (parent)
{
    HighlightingRule rule;
    keywordFormat.setForeground(QColor(103, 216, 239));
    QStringList keywordPatterns;
    keywordPatterns << "\\badd\\b" << "\\baddi\\b" << "\\bsub\\b" << "\\bsrl\\b"
                    << "\\bsll\\b" << "\\blw\\b" << "\\bsw\\b" << "\\band\\b"
                    << "\\bor\\b" << "\\bnor\\b" << "\\bandi\\b" << "\\bori\\b"
                    << "\\bslt\\b" << "\\bslti\\b" << "\\bbeq\\b" << "\\bbne\\b"
                    << "\\bj\\b" << "\\bjal\\b" << "\\bjr\\b" << "\\bjalr\\b" << "\\bxor\\b"
                    << "\\baddu\\b" << "\\baddiu\\b" << "\\blui\\b";
    foreach(const QString& pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightRules.append(rule);
    }
    constantFormat.setForeground(QColor(172, 128, 255));
    QStringList constantPatterns;
    constantPatterns << "\\-[0-9]+" << "0x[0-9a-fA-F]+" <<"\\b[0-9]+\\b";
    foreach(const QString& pattern, constantPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = constantFormat;
        highlightRules.append(rule);
    }
    labelFormat.setForeground(QColor(249, 36, 114));
    QStringList labelPatterns;
    labelPatterns << "[0-9a-zA-Z_]+:";
    foreach(const QString& pattern, labelPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = labelFormat;
        highlightRules.append(rule);
    }
    registerFormat.setForeground(QColor(166, 226, 44));
    QStringList registerPatterns;
    registerPatterns << "\\$[a-zA-Z0-9]+";
    foreach(const QString& pattern, registerPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = registerFormat;
        highlightRules.append(rule);
    }

}

void Highlighter::highlightBlock(const QString &text){
    foreach (const HighlightingRule &rule, highlightRules) {//文本采用高亮规则
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
}
