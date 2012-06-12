#include "TextManager.h"

#include <Utils/Logger.hpp>
#include <QtXml/QtXml>

std::shared_ptr<TextManager> TextManager::mInstance = std::shared_ptr<TextManager> (new TextManager());

TextManager::~TextManager() {

}

void TextManager::initialize() {

}

void TextManager::deinitialize() {

}

bool TextManager::loadTexts(QString path) {
    QFile config_file("Text.xml");
    QDomDocument doc;

    // If the file doesn't exist...
    if(!config_file.open(QIODevice::ReadOnly)) {
        dt::Logger::get().info("The configuration file doesn't exist. Trying to create a new one.");
        return false;
    }

    if (doc.setContent(&config_file)) {
        QDomElement root = doc.documentElement();
        for (QDomElement Text_node = root.firstChildElement("Talk"); !Text_node.isNull(); Text_node = Text_node.nextSiblingElement()) {

            
            auto id = Text_node.firstChildElement("id");
            uint16_t index = id.text().toUInt();

            auto content_node = Text_node.firstChildElement("content");
            QString question_content = content_node.text();
                      
            mTexts.insert(std::pair<uint16_t, QString> (index, question_content));
        } 
    } else {
        dt::Logger::get().error("Failed to read from the configuration file.");
        return false;
    }
    return true;
}

QString TextManager::getText(uint16_t text_id) {
    return mTexts[text_id];
}

TextManager* TextManager::getInstance() {
    return mInstance.get();
}