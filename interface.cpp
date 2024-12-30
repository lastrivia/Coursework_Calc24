#include "interface.h"

namespace calc {

    QColor darkenColor(const QColor &color, int factor) {
        // 将颜色变暗，factor 越大颜色越暗
        int r = color.red() - factor;
        int g = color.green() - factor;
        int b = color.blue() - factor;
        return QColor(qMax(0, r), qMax(0, g), qMax(0, b));  // 确保颜色值不小于 0
    }
}