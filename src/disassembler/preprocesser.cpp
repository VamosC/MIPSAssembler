#include "preprocesser.h"
#include <QDebug>
PreProcesser::PreProcesser()
{}

void PreProcesser::process(const QStringList &input) {
    QString test("ggg;11");
    QString s(input.join(" "));
    QRegExp re("memory_initialization_radix\\s*=\\s*([0-9]+)\\s*;\\s*memory_initialization_vector\\s*=\\s*([^;]+);\\s*");
    re.indexIn(s);
    int radix = re.cap(1).toInt();
    QString inst(re.cap(2));
    inst = inst.trimmed();
    output = inst.split(',');
    for(int i = 0; i < output.size(); i++) {
        if(radix == 2)
            output[i] = _32bitToHexFormat(output[i].trimmed());
        else if(radix == 16)
            output[i] = output[i].trimmed();
    }
}

QStringList PreProcesser::getRes() const {
    return output;
}
