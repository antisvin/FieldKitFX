#include "ui/page.h"
#include "engine/engine.h"

namespace fieldkitfx {

void UiFxPage::onButtonPressed() {
    parameter_controller.setParams2Page(this);
    fromSettings();
}

// void UiVcoPage::render() {
//}

// void UiLooperPage::render() {
//}

// void UiModulationPage::syncState() {
//}

// void UiVoctPage::syncState() {
//}

// void UiVoctPage::render() {
//}

/*
void UiVoctPage::onButtonPressed() {
}

void UiVoctPage::onButtonDepressed() {
}
*/

// void UiVolumePage::syncState() {
//}

// void UiVolumePage::render() {
//}

void UiVolumePage::onLoopButtonPressed(bool shouldSave) {
}

void UiVolumePage::onButtonPressed() {
}

void UiVolumePage::onButtonDepressed() {
}

void UiPresetRandomPage::onLoopButtonPressed(bool shouldSave) {
    // Choose random VCF
    // Chose random FX1-4
    // VCF and effects correlate - enabling one should reduce probability
    // that next effect is also enabledd - and vice versa
    // Choose parameters for all enabled VCF/FX
    // Choose looper type
    // Choose modulator
}

/*
void UiPresetSavePage::syncState() {
}

void UiPresetLoadPage::syncState() {
}
*/

}