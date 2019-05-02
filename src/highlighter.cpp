#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter (parent)
{
    HighlightingRule rule;

    labelFormat.setForeground(QColor(249, 36, 114));
    QStringList labelPatterns;
    labelPatterns << "[a-zA-Z_][0-9a-zA-Z_]*:" << "[a-zA-Z_][0-9a-zA-Z_]*";
    foreach(const QString& pattern, labelPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = labelFormat;
        highlightRules.append(rule);
    }

    constantFormat.setForeground(QColor(172, 128, 255));
    QStringList constantPatterns;
    constantPatterns << "\\-[0-9]+" << "0x[0-9a-fA-F]+" <<"\\b[0-9a-fA-F]+\\b";
    foreach(const QString& pattern, constantPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = constantFormat;
        highlightRules.append(rule);
    }

    registerFormat.setForeground(QColor(166, 226, 44));
    QStringList registerPatterns;
    registerPatterns << "\\$zero" << "\\$ZERO" << "\\$at" << "\\$AT"
                     << "\\$v0" << "\\$V0" << "\\$v1" << "\\$V1"
                     << "\\$a0" << "\\$A0" << "\\$a1" << "\\$A1"
                     << "\\$a2" << "\\$A2" << "\\$a3" << "\\$A3"
                     << "\\$t0" << "\\$T0" << "\\$t1" << "\\$T1"
                     << "\\$t2" << "\\$T2" << "\\$t3" << "\\$T3"
                     << "\\$t4" << "\\$T4" << "\\$t5" << "\\$T5"
                     << "\\$t6" << "\\$T6" << "\\$t7" << "\\$T7"
                     << "\\$s0" << "\\$S0" << "\\$s1" << "\\$S1"
                     << "\\$s2" << "\\$S2" << "\\$s3" << "\\$S3"
                     << "\\$s4" << "\\$S4" << "\\$s5" << "\\$S5"
                     << "\\$s6" << "\\$S6" << "\\$s7" << "\\$S7"
                     << "\\$t8" << "\\$T8" << "\\$t9" << "\\$T9"
                     << "\\$k0" << "\\$K0" << "\\$k1" << "\\$K1"
                     << "\\$gp" << "\\$GP" << "\\$sp" << "\\$SP"
                     << "\\$fp" << "\\$FP" << "\\$ra" << "\\$RA";
    foreach(const QString& pattern, registerPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = registerFormat;
        highlightRules.append(rule);
    }
    keywordFormat.setForeground(QColor(103, 216, 239));
    QStringList keywordPatterns;
    keywordPatterns << "\\badd\\b" << "\\baddi\\b" << "\\baddu\\b" << "\\baddiu\\b"
                    << "\\band\\b" << "\\bandi\\b" << "\\bclo\\b" << "\\bclz\\b"
                    << "\\bdiv\\b" << "\\bdivu\\b" << "\\bmult\\b" << "\\bmultu\\b"
                    << "\\bmul\\b" << "\\bmadd\\b" << "\\bmaddu\\b" << "\\bmsub\\b"
                    << "\\bmsubu\\b" << "\\bnor\\b" << "\\bor\\b" << "\\bori\\b"
                    << "\\bsll\\b" << "\\bsllv\\b" << "\\bsra\\b" << "\\bsrav\\b"
                    << "\\bsrl\\b" << "\\bsrlv\\b" << "\\bsub\\b" << "\\bsubu\\b"
                    << "\\bxor\\b" << "\\bxori\\b" << "\\blui\\b" << "\\bslt\\b"
                    << "\\bsltu\\b" << "\\bslti\\b" << "\\bsltiu\\b" << "\\bbeq\\b"
                    << "\\bbgez\\b" << "\\bbgezal\\b" << "\\bbgtz\\b" << "\\bblez\\b"
                    << "\\bbltzal\\b" << "\\bltz\\b" << "\\bbne\\b" << "\\bj\\b"
                    << "\\bjal\\b" << "\\bjalr\\b" << "\\bjr\\b" << "\\bteq\\b"
                    << "\\bteqi\\b" << "\\btne\\b" << "\\btnei\\b" << "\\btge\\b"
                    << "\\btgeu\\b" << "\\btgei\\b" << "\\btgeiu\\b" << "\\btlt\\b"
                    << "\\btltu\\b" << "\\btlti\\b" << "\\btltit\\b" << "\\blb\\b"
                    << "\\blbu\\b" << "\\blh\\b" << "\\blhu\\b" << "\\blw\\b"
                    << "\\bsw\\b" << "\\blwl\\b" << "\\blwr\\b" << "\\bll\\b"
                    << "\\bsb\\b" << "\\bsh\\b" << "\\bswl\\b" << "\\bswr\\b"
                    << "\\bsc\\b" << "\\bmfhi\\b" << "\\bmflo\\b" << "\\bmtlo\\b"
                    << "\\bmthi\\b" << "\\bmfc0\\b" << "\\bmtc0\\b" << "\\bmovn\\b"
                    << "\\bmovz\\b" << "\\beret\\b" << "\\bsyscall\\b" << "\\bnop\\b";
    foreach(const QString& pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightRules.append(rule);
    }

    commentFormat.setForeground(QColor(116, 112, 93));
    QStringList commentPatterns;
    commentPatterns << "//[^\n]*";
    foreach(const QString& pattern, commentPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = commentFormat;
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
