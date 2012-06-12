#include <QASetting.h>

QASetting::QASetting() {
    mIsEnable = false;
}

bool QASetting::getIsQAEnable() const {
    return mIsEnable;
}

void QASetting::setIsQAEnable(const bool is_enable) {
    mIsEnable = is_enable;
}