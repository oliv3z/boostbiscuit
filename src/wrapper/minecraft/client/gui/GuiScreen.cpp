
#include "GuiScreen.h"

#include "wrapper/minecraft/client/Minecraft.h"


std::shared_ptr<java_class> wrapper::GuiScreen::GuiScreen_class = nullptr;
std::shared_ptr<java_class> wrapper::GuiScreen::GuiChat_class = nullptr;
std::shared_ptr<java_class> wrapper::GuiScreen::GuiInventory_class = nullptr;
std::shared_ptr<java_class> wrapper::GuiScreen::GuiChest_class = nullptr;


wrapper::GuiScreen::GuiScreen(JNIEnv *env, jobject obj) : java_object(env, obj) {
    GuiScreen_class = GuiScreen_class ? GuiScreen_class : std::make_shared<java_class>(env, "net/minecraft/client/gui/GuiScreen");
    GuiChat_class = GuiChat_class ? GuiChat_class : std::make_shared<java_class>(env, "net/minecraft/client/gui/GuiChat");
    GuiInventory_class = GuiInventory_class ? GuiInventory_class : std::make_shared<java_class>(env, "net/minecraft/client/gui/inventory/GuiInventory");
    GuiChest_class = GuiChest_class ? GuiChest_class : std::make_shared<java_class>(env, "net/minecraft/client/gui/inventory/GuiChest");
}
