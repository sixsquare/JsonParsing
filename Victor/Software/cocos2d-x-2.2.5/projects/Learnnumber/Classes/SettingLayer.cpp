//  HelloCocosBuilderLayer.cpp
//  Demo
//  Created by Neil D on 20/05/13.
#include "SettingLayer.h"
#include "homeLayerLoader.h"
#include "GamePlayLayerLoader.h"
#include "GamePlayLayer.h"
#include "MainLayer.h"
#include "MainLayerLoader.h"
#include "CCMask.h"
#include "SimpleAudioEngine.h"
#include "CCSwipeGestureRecognizer.h"
#include "CCActivityIndicator.h"
// iOS Includes
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
SettingLayer::SettingLayer():
lbl_profile(NULL),
lbl_instract_1(NULL),
lbl_instract_2(NULL),
lbl_about(NULL),
lbl_sound(NULL),
lbl_vibration(NULL),
sprt_text_bg(NULL),
sprt_img_profile(NULL),
is_authenticated(false),
isbg_on(0.0f),
issoundFX_on(0.0f),
lbl_password(NULL),
layer_in_app(NULL),itm_remove_ads(NULL),
restore_purchase(NULL),
lbladvertisement(NULL),
layer_seprator(NULL)
{
    
}
SettingLayer::~SettingLayer()
{
    
    
}
void SettingLayer::onEnter()
{
     ObjCCalls::full_screen_banner(true);
    //if there is an in app purchase than hode remove ads button..
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("isfullpurchased")==true)
    {
        itm_remove_ads->setVisible(false);
        restore_purchase->setVisible(false);
        layer_seprator->setVisible(false);
        lbladvertisement->setVisible(false);
        /*CCNode *node=this->getChildByTag(113355);
        if (node)
        {
            CCNode *node1=node->getChildByTag(987654);
            CCNode *node2=node1->getChildByTag(226644);
            node2->setVisible(false);
            
        }*/
    }
    
    layer_pwd=(CCLayerColor*)this->getChildByTag(13579);
    this->reorderChild(layer_pwd,102390);
    layer_in_app=(CCLayerColor*)this->getChildByTag(29684);
    this->layer_in_app->setVisible(false);
    if (CCUserDefault::sharedUserDefault()->getStringForKey("password").length()!=0)
    {
        //password field...
        sprt_password=CCScale9Sprite::create("password_text_field_bg.png");
        is_authenticated=false;

        //CCNode *nd=this->getChildByTag(6856);
        m_pEditpassword_enter =CCEditBox::create(CCSizeMake(sprt_password->getContentSize().width, sprt_password->getContentSize().height),sprt_password);
        m_pEditpassword_enter->setFont("Station-Medium", 30);
        m_pEditpassword_enter->setFontSize(50);
        m_pEditpassword_enter->setFontColor(ccORANGE);
        m_pEditpassword_enter->setPlaceHolder("Enter Password");
        m_pEditpassword_enter->setInputFlag(kEditBoxInputFlagPassword);
        m_pEditpassword_enter->setPlaceholderFontColor(ccORANGE);
        m_pEditpassword_enter->setMaxLength(8);
        m_pEditpassword_enter->setReturnType(kKeyboardReturnTypeGo);
        m_pEditpassword_enter->setDelegate(this);
        this->addChild(m_pEditpassword_enter, 102390,98234);
        
        CCSize s=CCDirector::sharedDirector()->getWinSize();
        m_pEditpassword_enter->setPosition(ccp(s.width/2, 3*s.height/4));
    }
    else
    {
        is_authenticated=true;
        layer_pwd->setVisible(false);
    }
    this->set_textfield();
    this->set_slider();
    this->set_user_profile();
    if(CCDirector::sharedDirector()->getWinSize().width==2048)
    {
        //lbl_profile->setFontSize(65);
        lbl_instract_1->setFontSize(40);
        lbl_instract_2->setFontSize(40);
       // lbl_about->setFontSize(65);
        lbl_sound->setFontSize(55);
        lbl_password->setFontSize(65);
        lbl_vibration->setFontSize(55);
    }
    else
    {
        //lbl_profile->setFontSize(32);
        lbl_instract_1->setFontSize(20);
        lbl_instract_2->setFontSize(20);
        //lbl_about->setFontSize(34);
        lbl_sound->setFontSize(34);
        lbl_password->setFontSize(34);
        lbl_vibration->setFontSize(34);
    }
    CCLayer::onEnter();
}
void SettingLayer::set_user_profile()
{
    //set user profile if exist
    if (CCUserDefault::sharedUserDefault()->getStringForKey("profile_name").size())
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("has_profile", true);
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("has_profile", false);
    }
    CCUserDefault::sharedUserDefault()->flush();
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "profile_image.png";
    if (CCFileUtils::sharedFileUtils()->isFileExist(fullPath))
    {
        CCSprite *profile_pic=CCSprite::create(fullPath.c_str());
        profile_pic->setRotation(180);
        profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        masked_profile = CCMask::create(sprt_img_profile , profile_pic);
        masked_profile->setPosition(sprt_img_profile->getPosition());
        this->addChild(masked_profile);
        masked_profile->setPosition(sprt_img_profile->getPosition());
        m_pEditName->setText(CCUserDefault::sharedUserDefault()->getStringForKey("profile_name").c_str());
    }
    else
    {
        CCSprite *profile_pic=CCSprite::create("profilepic_bg.png");
        profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        masked_profile = CCMask::create(sprt_img_profile , profile_pic);
        masked_profile->setPosition(sprt_img_profile->getPosition());
        this->addChild(masked_profile);
        masked_profile->setPosition(sprt_img_profile->getPosition());
        m_pEditName->setText(CCUserDefault::sharedUserDefault()->getStringForKey("profile_name").c_str());
    }
}
void SettingLayer::set_slider()
{
    isbg_on=CCUserDefault::sharedUserDefault()->getFloatForKey("bgmusic",1.0);
    issoundFX_on=CCUserDefault::sharedUserDefault()->getFloatForKey("soundfx",1.0);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(issoundFX_on);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(isbg_on);
    CCNode *ndsound=this->getChildByTag(677);
    CCControlSlider *slidersound = CCControlSlider::create("sound_volume_bg.png","sound_volume_bg.png","volume_up_down.png");
    slidersound->setScale(0.9);
    slidersound->setMinimumValue(0.0f); // Sets the min value of range
    slidersound->setMaximumValue(1.0f); // Sets the max value of range
    float v=isbg_on*0.6;
    v=v+0.2;
    slidersound->setValue(v);
    slidersound->setMaximumAllowedValue(0.80f);
    slidersound->setMinimumAllowedValue(0.20f);
    slidersound->setPosition(ccp(ndsound->getPositionX() ,ndsound->getPositionY()-(slidersound->getContentSize().height*2.8)));
    slidersound->setTag(1);
    slidersound->setRotation(-90);
    // When the value of the slider will change, the given selector will be call
    slidersound->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::valueChanged), CCControlEventValueChanged);
    this->addChild(slidersound,768);

    CCNode *ndmusic=this->getChildByTag(766);
    CCControlSlider *slidermusic = CCControlSlider::create("music_volume_bg.png","music_volume_bg.png","volume_up_down.png");
    slidermusic->setMinimumValue(0.0f); // Sets the min value of range
    slidermusic->setMaximumValue(1.0f); // Sets the max value of range
    slidermusic->setScale(0.9);
    slidermusic->setMaximumAllowedValue(0.80f);
    slidermusic->setMinimumAllowedValue(0.20f);
    float v1=issoundFX_on*0.6;
    v1=v1+0.2;
    slidermusic->setValue(v1);
    slidermusic->setPosition(ccp(ndmusic->getPositionX() ,ndsound->getPositionY()-(slidersound->getContentSize().height*2.8)));
    slidermusic->setTag(2);
    slidermusic->setRotation(-90);
    // When the value of the slider will change, the given selector will be call
    slidermusic->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::valueChanged), CCControlEventValueChanged);
    this->addChild(slidermusic,769);
    
}
void SettingLayer::valueChanged(CCObject *sender, CCControlEvent controlEvent)
{
    CCControlSlider* pSlider = (CCControlSlider*)sender;
    
	if(pSlider->getTag() == 1)
    {
        float v=pSlider->getValue()-0.2;
        float v1=100*v/0.6;
        CCUserDefault::sharedUserDefault()->setFloatForKey("bgmusic", v1/100);
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume( v1/100);
        CCUserDefault::sharedUserDefault()->flush();
    }
	if(pSlider->getTag() == 2)
    {
        float v=pSlider->getValue()-0.2;
        float v1=100*v/0.6;
        CCUserDefault::sharedUserDefault()->setFloatForKey("soundfx", v1/100);
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(v1/100);
        CCUserDefault::sharedUserDefault()->flush();
    }
}
#pragma mark -
#pragma mark action methods
#pragma mark -
void SettingLayer::onDidSwipe(cocos2d::CCObject * pSender)
{
    CCSwipe * swipe = (CCSwipe*)pSender;
    if (swipe->direction==kSwipeGestureRecognizerDirectionRight)
    {
        
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionLeft)
    {
        
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionUp)
    {
        
    }
    else  if (swipe->direction==kSwipeGestureRecognizerDirectionDown)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        CCDirector::sharedDirector()->popScene();
    }
}
void SettingLayer::onProfilePicClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    if (is_authenticated==false)
        return;
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    ImagePicker::pickImage(this, CCPointZero,this);
}
void SettingLayer::onRestorepurchasedClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    ObjCCalls::get_user_image();
    this->schedule(schedule_selector(SettingLayer::update_for_iap));
}
void SettingLayer::onRemoveAdsClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    //for some hard fucking reason ObjCCalls class don't accept new metod so used existing
    //sorry about that...
    this->layer_in_app->setVisible(true);
    this->reorderChild(this->layer_in_app,102390);
    //
}
void SettingLayer::oncrossClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCDirector::sharedDirector()->popScene();
}
void SettingLayer::onYesAdsremoveClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->layer_in_app->setVisible(false);
    ObjCCalls::get_user_image();
    this->schedule(schedule_selector(SettingLayer::update_for_iap));
}
void SettingLayer::update_for_iap(CCTime dt)
{
    CCLOG("in app call back check");
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("isfullpurchased")==true)
    {
        CCLOG("purchased");
        itm_remove_ads->setVisible(false);
        restore_purchase->setVisible(false);
        lbladvertisement->setVisible(false);
        layer_seprator->setVisible(false);
        this->unschedule(schedule_selector(SettingLayer::update_for_iap));
    }
    else
    {
        CCLOG("purchasing....");
    }
}
void SettingLayer::onNoAdsremoveClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->layer_in_app->setVisible(false);
}
#pragma mark -
#pragma mark - imagepicker delegate
#pragma mark -
void SettingLayer::didFinishPickingWithResult(bool result)
{
    if(result)
    {
        CCSprite *user_image=ImagePicker::getLastSprite();
        user_image->setPosition(ccp(user_image->getContentSize().width/2, user_image->getContentSize().height/2));
        masked_profile->changeObject(user_image);
        
        if (user_image)
        {
            CCRenderTexture* texture = CCRenderTexture::create((int)user_image->getContentSize().width, (int)user_image->getContentSize().height);
            texture->setPosition(masked_profile->getPosition());
            texture->begin();
            user_image->visit();
            texture->end();
            texture->saveToFile("profile_image.png", kCCImageFormatPNG);
        }
        
    }
}
#pragma mark -
#pragma mark Edit Box
#pragma mark -
void SettingLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}
void SettingLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}
void SettingLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    if (editBox->getTag()==98234)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("storePassword", editBox->getText());
        CCUserDefault::sharedUserDefault()->flush();
    }

    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}
void SettingLayer::editBoxReturn(CCEditBox* editBox)
{
    if (editBox->getTag()==919)
    {
        CCLOG("%s",editBox->getText());
        CCUserDefault::sharedUserDefault()->setStringForKey("password", editBox->getText());
        CCUserDefault::sharedUserDefault()->flush();
    }
    else if (editBox->getTag()==98234)
    {
        if(strcmp(editBox->getText(),CCUserDefault::sharedUserDefault()->getStringForKey("password").c_str())==0)
        {
            if(CCUserDefault::sharedUserDefault()->getStringForKey("storePassword").length()>0)
            m_pEditpassword->setText(CCUserDefault::sharedUserDefault()->getStringForKey("storePassword").c_str());
            layer_pwd->setVisible(false);
            editBox->setVisible(false);
            is_authenticated=true;
        }
        else
        {
            CCActionInterval  *tint=CCTintTo::create(0.3, 255, 0, 0);
            CCActionInterval  *tint_reverse=CCTintTo::create(0.3, 255,255,255);
            m_pEditpassword_enter->runAction(CCSequence::create(tint,tint_reverse,NULL));
            //nothing..
        }
    }
    else
    {
        if(CCString::createWithFormat("%s",editBox->getText())->length()>0)
        {
            
        }
        else
        {
            return;
        }
        CCDictionary *dict=CCDictionary::create();
        //set user profile if exist
        std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
        std::string fullPath = writablePath_temp + "profile_image.png";
        if(CCFileUtils::sharedFileUtils()->isFileExist(fullPath))
        {
            //CCLog("yes user profile pic available");
            dict->setObject(CCString::createWithFormat("%s","profile_image.png"), "user_profile");
        }
        else
        {
            
        }
        if (CCUserDefault::sharedUserDefault()->getStringForKey("profile_name").length()>0)
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("profile_name", editBox->getText());
            CCUserDefault::sharedUserDefault()->flush();
            dict->setObject(CCString::createWithFormat("%s",editBox->getText()), "user_name");
            this->add_on_Player_list(dict,true);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("profile_name", editBox->getText());
            CCUserDefault::sharedUserDefault()->flush();
            dict->setObject(CCString::createWithFormat("%s",editBox->getText()), "user_name");
            this->add_on_Player_list(dict,false);
        }
    }
}
void SettingLayer::add_on_Player_list(CCDictionary *dict , bool is_updating)
{
    CCArray *arr_users=CCArray::create();
    arr_users->retain();
    arr_users=this->get_userlist();
    if (arr_users->count()>1)
    {
        if (is_updating)
        {
            arr_users->replaceObjectAtIndex(0, dict);
        }
        else
        {
            arr_users->addObject(dict);
            arr_users->exchangeObjectAtIndex(0,arr_users->count());
        }
    }
    else
    {
        if (is_updating) 
        {
            arr_users->insertObject(dict, 0);
        }
        else
        {
            arr_users->insertObject(dict, 0);
        }
    }

    CCDictionary *root = CCDictionary::create();
    root->setObject(arr_users, "array");
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "UserList.plist";
    if(root->writeToFile(fullPath.c_str()))
    {
        
    }
}
CCArray* SettingLayer::get_userlist()
{
    CCArray *arr_users;
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "UserList.plist";
    if (CCFileUtils::sharedFileUtils()->isFileExist(fullPath))
    {
        CCDictionary *dict=CCDictionary::createWithContentsOfFile(fullPath.c_str());
        dict->retain();
        arr_users=(CCArray*)dict->objectForKey("array");
    }
    else
    {
        arr_users=CCArray::createWithContentsOfFileThreadSafe("Published-iOS/UserList.plist");
        CCDictionary *root = CCDictionary::create();
        root->setObject(arr_users, "array");
        if(root->writeToFile(fullPath.c_str()))
        {
            //CCLog("see the plist file at %s", fullPath.c_str());
        }
        else
        {
            //CCLog("write plist file failed");
        }
        //CCLog("external file path = %s", fullPath.c_str());
    }
    return arr_users;
}
void SettingLayer::set_textfield()
{
    CCScale9Sprite *sprt=CCScale9Sprite::create("text_field_bg.png");
    sprt->setOpacity(0);
    m_pEditName =CCEditBox::create(CCSizeMake(3*sprt_text_bg->getContentSize().width/4, sprt_text_bg->getContentSize().height),sprt);
    m_pEditName->setFont("Station-Medium", 30);
    m_pEditName->setFontSize(35);
    m_pEditName->setFontColor(ccORANGE);
    m_pEditName->setPlaceHolder("Name");
    m_pEditName->setPlaceholderFontColor(ccORANGE);
    m_pEditName->setMaxLength(8);
    m_pEditName->setReturnType(kKeyboardReturnTypeGo);
    m_pEditName->setDelegate(this);
    this->addChild(m_pEditName, 1,9);
    m_pEditName->setPosition(sprt_text_bg->getPosition());
    
    //password field...
    CCScale9Sprite *sprt1=CCScale9Sprite::create("text_field_bg.png");
    sprt1->setOpacity(0);
    
    CCNode *nd=this->getChildByTag(6856);
    m_pEditpassword =CCEditBox::create(CCSizeMake(3.5*nd->getContentSize().width/4, nd->getContentSize().height),sprt1);
    m_pEditpassword->setFont("Station-Medium", 30);
    m_pEditpassword->setFontSize(35);
    m_pEditpassword->setFontColor(ccORANGE);
    m_pEditpassword->setPlaceHolder("Password");
    m_pEditpassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditpassword->setPlaceholderFontColor(ccORANGE);
    
    m_pEditpassword->setMaxLength(8);
    m_pEditpassword->setReturnType(kKeyboardReturnTypeGo);
    m_pEditpassword->setDelegate(this);
    this->addChild(m_pEditpassword, 1,919);
    m_pEditpassword->setPosition(nd->getPosition());
    m_pEditpassword->setPosition(ccpAdd(nd->getPosition(), ccp(0, 0)));
}
#pragma mark -
#pragma mark CCBI methods
#pragma mark -
void SettingLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    
}
SEL_CallFuncN SettingLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}
SEL_MenuHandler SettingLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onProfilePicClicked", SettingLayer::onProfilePicClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRestorepurchasedClicked", SettingLayer::onRestorepurchasedClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRemoveAdsClicked", SettingLayer::onRemoveAdsClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "oncrossClicked", SettingLayer::oncrossClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onYesAdsremoveClicked", SettingLayer::onYesAdsremoveClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNoAdsremoveClicked", SettingLayer::onNoAdsremoveClicked);
    return NULL;
}
SEL_CCControlHandler SettingLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool SettingLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_instract_1", CCLabelTTF *, this->lbl_instract_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_instract_2", CCLabelTTF *, this->lbl_instract_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_about", CCLabelTTF *, this->lbl_about);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_sound", CCLabelTTF *, this->lbl_sound);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_vibration", CCLabelTTF *, this->lbl_vibration);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_password", CCLabelTTF *, this->lbl_password);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_text_bg", CCSprite *, this->sprt_text_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_img_profile", CCSprite *, this->sprt_img_profile);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "itm_remove_ads", CCMenuItemImage *, this->itm_remove_ads);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "restore_purchase", CCMenuItemImage *, this->restore_purchase);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbladvertisement", CCLabelTTF *, this->lbladvertisement);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_seprator",CCLayerColor*, this->layer_seprator);
    return false;
}
bool SettingLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    //CCLog("onAssignCCBCustomProperty");
    bool bRet = false;
    return bRet;
}