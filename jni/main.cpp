#include <dlfcn.h>
#include <jni.h>
#include <string>
#include <memory>
#include <android/log.h>
#include "mcpelauncher.h"

#include "mcpe/util/I18n.h"
#include "mcpe/client/MinecraftClient.h"
#include "mcpe/client/gui/HeaderButton.h"
#include "mcpe/client/gui/TextBox.h"
#include "mcpe/client/gui/Font.h"
#include "mcpe/client/gui/Label.h"
#include "mcpe/client/gui/TButton.h"
#include "mcpe/client/gui/ImageButton.h"
#include "mcpe/client/gui/screens/CreateWorldScreen.h"

using namespace std;

class FlatLevelSource {
    public:
    static string DEFAULT_LAYERS;
};

void (*create_button)(CreateWorldScreen*, Button*);
void create_button_hook(CreateWorldScreen* thiz, Button* btn) {
    if(thiz->isEditMode()) {
        create_button(thiz, btn);
        return;
    }
    
    if(btn->displayString == thiz->worldtypeBtns[2]->displayString ||
      (btn->displayString == thiz->deleteOrEditBtn->displayString && thiz->selectedWorldtypeBtn->displayString == thiz->worldtypeBtns[2]->displayString)) {
        vector<string> vec;
        thiz->generatorType = 2;
        thiz->selectedWorldtypeBtn = thiz->worldtypeBtns[2].get();
        thiz->_isFlat = true;
        thiz->levelSeedLabel->width = thiz->width;
        thiz->levelSeedLabel->text = I18n::get("createWorld.customize.flat.title", vec);
        thiz->levelSeedTextbox->setName(I18n::get("createWorld.customize.flat.title", vec));
        thiz->levelSeedTextbox->setText("[7,3,3,2]");
        thiz->gamemodeBtns[0]->enabled = true;
        thiz->gamemodeBtns[1]->enabled = true;
        return;
    } else if(btn->displayString != thiz->createWorldBtn->displayString && thiz->gamemodeBtns[0]->displayString != btn->displayString && thiz->gamemodeBtns[1]->displayString != btn->displayString) {
        vector<string> vec;
        thiz->levelSeedTextbox->setText("");
        thiz->levelSeedTextbox->setName(I18n::get("createWorldScreen.levelSeed", vec));
        thiz->levelSeedLabel->text = I18n::get("createWorldScreen.levelSeed", vec);
    }
    
    if(btn->displayString == thiz->createWorldBtn->displayString && thiz->levelSeedTextbox->getText() != "") {
        if(thiz->isFlat())
            FlatLevelSource::DEFAULT_LAYERS = thiz->levelSeedTextbox->getText();
        else
            FlatLevelSource::DEFAULT_LAYERS = "[7,3,3,2]";
    } else
        FlatLevelSource::DEFAULT_LAYERS = "[7,3,3,2]";
    
    create_button(thiz, btn);
}

int (*create_gamemode)(CreateWorldScreen*);
int create_gamemode_hook(CreateWorldScreen* thiz) {
    return thiz->gamemode;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    mcpelauncher_hook((void*) &CreateWorldScreen::buttonClicked, (void*) &create_button_hook,   (void**) &create_button);
    mcpelauncher_hook((void*) &CreateWorldScreen::_getGameMode,  (void*) &create_gamemode_hook, (void**) &create_gamemode);
    mcpelauncher_hook((void*) &CreateWorldScreen::render,        (void*) &create_render_hook,   (void**) &create_render);
    
    return JNI_VERSION_1_2;
}
