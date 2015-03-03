//  HelloCocosBuilderLayer.cpp
//  Created by Neil D on 20/05/13.
#include "SelectPlayerLayer.h"
#include "LeaderBoardNodeLoader.h"
#include "homeLayerLoader.h"
#include "GamePlayLayerLoader.h"
#include "GamePlayLayer.h"
#include "MainLayer.h"
#include "MainLayerLoader.h"
#include "CustomTableViewCell.h"
#include "ImagePicker.h"
#include "SimpleAudioEngine.h"
#include  "InfoLayerLoader.h"
#include "CountNumberLayer.h"
#include "CCSwipeGestureRecognizer.h"
#include "SettingLayerLoader.h"
#include "SettingLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
SelectPlayerLayer::SelectPlayerLayer():mAnimationManager(NULL),
lbl_title(NULL),
lbl_addnew_user(NULL),
lbl_instr_1(NULL),
lbl_instr_2(NULL),
lbl_select_from(NULL),
bg_text_field(NULL),
menu_img_profile(NULL),
sprt_img_profile(NULL),
existing_users(NULL),
masked(NULL),
sprt_image_picker(NULL),is_info_showing(false)
{
    existing_users=CCArray::create();
    existing_users->retain();
}
SelectPlayerLayer::~SelectPlayerLayer()
{
    CC_SAFE_RELEASE(mAnimationManager);
}
void SelectPlayerLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    if (CCDirector::sharedDirector()->getWinSize().width==2048)
    {
        lbl_title->setFontSize(90);
        lbl_addnew_user->setFontSize(64);
        lbl_instr_1->setFontSize(40);
        lbl_instr_2->setFontSize(40);
        lbl_select_from->setFontSize(64);
    }
    else
    {
        lbl_title->setFontSize(60);
        lbl_addnew_user->setFontSize(40);
        lbl_instr_1->setFontSize(25);
        lbl_instr_2->setFontSize(25);
        lbl_select_from->setFontSize(40);
    }
    this->set_textfield();
    CCSprite *user_image=CCSprite::create("place holder.png");
    user_image->setPosition(ccp(user_image->getContentSize().width/2, user_image->getContentSize().height/2));
    user_image->setScale(1.1);
    masked = CCMask::create(sprt_img_profile , user_image);
    masked->setPosition(sprt_img_profile->getPosition());
    this->addChild(masked);
    masked->setScale(1.1);
    this->add_user_tableview();
}
void SelectPlayerLayer::add_user_tableview()
{
    winSize=CCDirector::sharedDirector()->getWinSize();
    CCSprite *sprt_bg=CCSprite::create("user_select_bg.png");
    tableView = CCTableView::create(this, CCSizeMake(sprt_bg->getContentSize().width*2,winSize.height-lbl_select_from->getContentSize().height*3));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(CCPointMake(lbl_select_from->getPositionX()-lbl_select_from->getContentSize().width,0));
    existing_users=this->get_userlist();
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView);
    tableView->reloadData();
}
void SelectPlayerLayer::onEnter()
{
    CCLayer::onEnter();
    ObjCCalls::full_screen_banner(true);
    
    //tableView->setBounceable(false);
    m_pEditName->setText("");
    tableView->reloadData();
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("isfullpurchased")==true)
    {
        
    }
    else
    {
        //ObjCCalls::full_screen_banner(false);
        int ads_count=CCUserDefault::sharedUserDefault()->getIntegerForKey("ads_count", 1);
        if (ads_count%10==0)
            ads_count=9;
        switch (ads_count)
        {
            case 2:
                display_pop();
                break;
            case 5:
                display_pop();
                break;
            case 9:
                display_pop();
                break;
            default:
                //wtf no ads....!!!
                break;
        }
    }
    
}
void SelectPlayerLayer::display_pop()
{
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("should_display_ads")==0)
    {
        
    }
    else
    {
        //        revmob::RevMob *revmob = revmob::RevMob::SharedInstance();
        //        revmob->ShowFullscreen();
        ObjCCalls::full_screen_banner_revmob(true);
        CCUserDefault::sharedUserDefault()->setIntegerForKey("should_display_ads",0);
    }
    
}
void SelectPlayerLayer::set_textfield()
{
    CCScale9Sprite *sprt=CCScale9Sprite::create("text_field_bg.png");
    sprt->setOpacity(0);
    m_pEditName =CCEditBox::create(CCSizeMake(3*bg_text_field->getContentSize().width/4, 45),sprt);
    m_pEditName->setFont("Station-Medium", 30);
    m_pEditName->setFontSize(35);
    m_pEditName->setFontColor(ccORANGE);
    m_pEditName->setPlaceHolder("Name");
    m_pEditName->setPlaceholderFontColor(ccORANGE);
    m_pEditName->setMaxLength(8);
    m_pEditName->setReturnType(kKeyboardReturnTypeGo);
    m_pEditName->setDelegate(this);
    this->addChild(m_pEditName, 1,9);
    m_pEditName->setPosition(bg_text_field->getPosition());
}
#pragma mark -
#pragma animationManaget
void SelectPlayerLayer::setAnimationManager(cocos2d::extension::CCBAnimationManager *animationManager)
{
    if (animationManager)
    {
        CC_SAFE_RELEASE_NULL(mAnimationManager);
        mAnimationManager = animationManager;
        CC_SAFE_RETAIN(animationManager);
    }
}
void SelectPlayerLayer::callback_info_show(CCNode* sender)
{
    //hide other objects
    lbl_title->setString("How To Play");
    m_pEditName->setVisible(false);
    m_pEditName->setEnabled(false);
    tableView->setVisible(false);
    masked->setVisible(false);
    this->reorderChild(m_pEditName, -1);
    this->getChildByTag(11)->getChildByTag(13)->setVisible(false);
}
void SelectPlayerLayer::callback_info_hide(CCNode* sender)
{
    //show other objects
    lbl_title->setString("Play The Game");
    m_pEditName->setVisible(true);
    m_pEditName->setEnabled(true);
    tableView->setVisible(true);
    masked->setVisible(true);
    this->reorderChild(m_pEditName,1);
    this->getChildByTag(11)->getChildByTag(13)->setVisible(true);
}
#pragma mark -
#pragma mark Action methods
#pragma mark -
void SelectPlayerLayer::onBackClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    //CCLog("back button clicked");
    if (is_info_showing==true)
    {
        is_info_showing=false;
        SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        CCBAnimationManager   *anim = dynamic_cast<CCBAnimationManager *>( this->getUserObject());
        anim->runAnimationsForSequenceNamed("infoScreenclose");
        
    }
    else
    {
        SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        Scene_common* pScene = new Scene_common();
        if (pScene)
        {
            pScene->ReplaceScene("MainLayer.ccbi","MainLayer", MainLayerLoader::loader(),this,true,callfuncO_selector(MainLayer::onDidSwipe));
            pScene->release();
        }
    }
}
void SelectPlayerLayer::onHomeClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    is_info_showing=true;
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    CCBAnimationManager   *anim = dynamic_cast<CCBAnimationManager *>( this->getUserObject());
    anim->runAnimationsForSequenceNamed("infoScreen");
    
    
}
void SelectPlayerLayer::onnew_user_clicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    if (is_info_showing==true)return;
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    for (int i=0;i<existing_users->count();i++)
    {
        CCDictionary *dict=(CCDictionary*)existing_users->objectAtIndex(i);
        if (strcmp(dict->valueForKey("user_name")->getCString(), m_pEditName->getText())==0)
        {
            CCActionInterval  *tint=CCTintTo::create(0.3, 255, 0, 0);
            CCActionInterval  *tint_reverse=CCTintTo::create(0.3, 255,255,255);
            bg_text_field->runAction(CCSequence::create(tint,tint_reverse,NULL));
            return;
        }
        
        
    }
    if (CCString::create(m_pEditName->getText())->length()==0)
    {
        CCActionInterval  *tint=CCTintTo::create(0.3, 255, 0, 0);
        CCActionInterval  *tint_reverse=CCTintTo::create(0.3, 255,255,255);
        bg_text_field->runAction(CCSequence::create(tint,tint_reverse,NULL));
        return;
    }
    CCDictionary *user=CCDictionary::create();
    user->setObject(CCString::create(m_pEditName->getText()), "user_name");
    if (sprt_image_picker)
    {
        CCRenderTexture* texture = CCRenderTexture::create((int)sprt_image_picker->getContentSize().width, (int)sprt_image_picker->getContentSize().height);
        texture->setPosition(masked->getPosition());
        texture->begin();
        sprt_image_picker->visit();
        texture->end();
        texture->saveToFile(CCString::createWithFormat("%d.png",existing_users->count()+1)->getCString(), kCCImageFormatPNG);
        user->setObject(CCString::createWithFormat("%d.png",existing_users->count()+1), "user_profile");
    }
    this->save_user(user);
    this->play_with_user(user);
}
void SelectPlayerLayer::onProfilePicClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    if (is_info_showing==true)return;
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    ImagePicker::pickImage(this, CCPointZero,this);
}
void SelectPlayerLayer::on_user_clicked(cocos2d::CCObject * pSender)
{
    if (is_info_showing==true)return;
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    CCTableViewCell *cell = (CCTableViewCell*)(((CCMenuItemSprite*)pSender)->getParent()->getParent());
    //CCLog("%d",cell->getIdx());
    existing_users->removeObjectAtIndex(cell->getIdx());
    CCDictionary *root = CCDictionary::create();
    root->setObject(existing_users, "array");
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "UserList.plist";
    if(root->writeToFile(fullPath.c_str()))
    {
        
    }
    tableView->reloadData();
    //CCDictionary *dict_user=(CCDictionary*)existing_users->objectAtIndex(cell->getIdx());
    //this->play_with_user(dict_user);
}
void SelectPlayerLayer::onDidSwipe(CCObject* pSender)
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
        SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        Scene_common* pScene = new Scene_common();
        if (pScene)
        {
            pScene->PushScene("SettingLayer.ccbi", "SettingLayer", SettingLayerLoader::loader(), this,true,callfuncO_selector(SettingLayer::onDidSwipe));
            pScene->autorelease();
        }
    }
    else  if (swipe->direction==kSwipeGestureRecognizerDirectionDown)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        Scene_common* pScene = new Scene_common();
        if (pScene)
        {
            pScene->ReplaceScene("MainLayer.ccbi","MainLayer", MainLayerLoader::loader(),this,true,callfuncO_selector(MainLayer::onDidSwipe));
            pScene->release();
        }
        
    }
}
#pragma mark -
#pragma mark model methods
#pragma mark -
void SelectPlayerLayer::play_with_user(CCDictionary *user)
{
    //CCLog("%s",user->valueForKey("user_name")->getCString());
    /// add user data in plist and move along for playing game...
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader=NULL;
    CountNumberLayer * node =NULL;
    ccNodeLoaderLibrary->registerCCNodeLoader("LeaderBoardNode", LeaderBoardNodeLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("CountNumberLayer", CountNumberLayerLoader::loader());
    ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    /* Read a ccbi file.*/
    //from each of the test scenes.
    node = (CountNumberLayer*)ccbReader->readNodeGraphFromFile("CountGameLayer.ccbi", this);
    ((CountNumberLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    CCScene * scene = CCScene::create();
    ccbReader->autorelease();
    if(node != NULL)
    {
        node->setuser_name((CCString*)user->valueForKey("user_name"));
        node->setuser_profile_path((CCString*)user->valueForKey("user_profile"));
        CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
        swipe->setTarget(node,callfuncO_selector(CountNumberLayer::onDidSwipe));
        swipe->setDirection(kSwipeGestureRecognizerDirectionRight | kSwipeGestureRecognizerDirectionLeft|kSwipeGestureRecognizerDirectionUp|kSwipeGestureRecognizerDirectionDown);
        swipe->setCancelsTouchesInView(true);
        scene->addChild(swipe);
        scene->addChild(node);
    }
    CCDirector::sharedDirector()->pushScene(scene);
}
CCArray*  SelectPlayerLayer::get_userlist()
{
    CCArray *arr_users;
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "UserList.plist";
    if(CCFileUtils::sharedFileUtils()->isFileExist(fullPath))
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
void SelectPlayerLayer::save_user(CCDictionary *user)
{
    existing_users->addObject(user);
    CCDictionary *root = CCDictionary::create();
    root->setObject(existing_users, "array");
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "UserList.plist";
    if(root->writeToFile(fullPath.c_str()))
    {
        
    }
    //CCLog("see the plist file at %s", fullPath.c_str());
    //else
    //CCLog("write plist file failed");
}
#pragma mark -
#pragma mark - imagepicker delegate
#pragma mark -
void  SelectPlayerLayer::didFinishPickingWithResult(bool result)
{
    if(result)
    {
        CCSprite *user_image=ImagePicker::getLastSprite();
        user_image->setPosition(ccp(user_image->getContentSize().width/2, user_image->getContentSize().height/2));
        masked = CCMask::create(sprt_img_profile , user_image);
        masked->setPosition(sprt_img_profile->getPosition());
        sprt_image_picker=ImagePicker::getLastSprite();
        this->addChild(masked);
        masked->setScale(1.14);
        masked->setPosition(sprt_img_profile->getPosition());
    }
}
#pragma mark -
#pragma mark Edit Box
#pragma mark -
void SelectPlayerLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidBegin !", editBox);
}
void SelectPlayerLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidEnd !", editBox);
}
void SelectPlayerLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    //CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}
void SelectPlayerLayer::editBoxReturn(CCEditBox* editBox)
{
    
}
#pragma mark -
#pragma mark table delegate
#pragma mark -
void SelectPlayerLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    //CCLog("cell touched at index: %i", cell->getIdx());
    //CCLog("%d",existing_users->count());
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    CCDictionary *dict_user=(CCDictionary*)existing_users->objectAtIndex(cell->getIdx());
    this->play_with_user(dict_user);
}
CCSize SelectPlayerLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    CCSprite *sprt_bg=CCSprite::create("user_select_bg.png");
    return CCSizeMake(sprt_bg->getContentSize().width+20, sprt_bg->getContentSize().height*1.3);
}
CCTableViewCell* SelectPlayerLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CustomTableViewCell *cell = (CustomTableViewCell*)table->dequeueCell();
    if (!cell)
    {
        cell = new CustomTableViewCell();
        cell->autorelease();
        
        CCSprite *sprt_bg=CCSprite::create("user_select_bg.png");
        sprt_bg->setPosition(CCPointMake(300*(winSize.width/1024), 0));
        cell->addChild(sprt_bg,0,121);
        //ccmenu for to go on play game screen...
        CCSprite *sprt_menu=CCSprite::create("user_delete.png");
        CCMenuItemSprite *menu_Sprt=CCMenuItemSprite::create(sprt_menu,sprt_menu, this, menu_selector(SelectPlayerLayer::on_user_clicked));
        menu_Sprt->setTag(idx+100);
        menu_Sprt->setScale(0.5);
        CCMenu *menu=CCMenu::create(menu_Sprt,NULL);
        cell->addChild(menu, 0, idx+1000);
        menu->setPosition(CCPointMake(sprt_bg->getPositionX()+sprt_bg->getContentSize().width/2-sprt_menu->getContentSize().width/2, 0));
        //cell data at current index
        CCDictionary *dict=(CCDictionary*)existing_users->objectAtIndex(idx);
        //user profile pic...
        std::string str_profile_pic =dict->valueForKey("user_profile")->getCString();
        std::string fullPath;
        bool is_rotated=false;
        std::string writablePath_temp= CCFileUtils::sharedFileUtils()->getWritablePath();
        fullPath = writablePath_temp + CCString::createWithFormat("%s",str_profile_pic.c_str())->getCString();
        CCSprite *profile_pic;
        if (CCFileUtils::sharedFileUtils()->isFileExist(fullPath.c_str())&&str_profile_pic.length()>0)
        {
            is_rotated=true;
            profile_pic=CCSprite::create(fullPath.c_str());
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        else
        {
            is_rotated=false;
            fullPath="place holder.png";
            profile_pic=CCSprite::create("place holder.png");
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        CCSprite *sprt_img_profile=CCSprite::create("profilepic_bg.png");
        sprt_img_profile->setPosition(sprt_bg->getPosition());
        CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
        cell->addChild(masked,11,124);
        masked->setPosition(CCPointMake(sprt_bg->getPositionX()-sprt_bg->getContentSize().width/2+100, 0));
        if (is_rotated)
            masked->setRotation(180);
        masked->setScale(0.35);
        //user name...
        std::string ret =dict->valueForKey("user_name")->getCString();
        if (strcmp(ret.c_str(),"")==0)
        {
            ret=CCString::createWithFormat("----")->getCString();
        }
        CCLabelTTF *name = CCLabelTTF::create(ret.c_str(), "Station-Medium", 50.0);
        name->setPosition(ccp(masked->getPositionX()+masked->getContentSize().width/2+25, sprt_bg->getPositionY()));
        name->setHorizontalAlignment(kCCTextAlignmentLeft);
        name->setColor(ccWHITE);
        name->setTag(123);
        cell->addChild(name);
    }
    else
    {
        CCLabelTTF *name = (CCLabelTTF*)cell->getChildByTag(123);
        CCDictionary *dict=(CCDictionary*)existing_users->objectAtIndex(idx);
        std::string ret =dict->valueForKey("user_name")->getCString();
        if (strcmp(ret.c_str(),"")==0)
        {
            ret=CCString::createWithFormat("--")->getCString();
        }
        name->setString(ret.c_str());
        std::string str_profile_pic =dict->valueForKey("user_profile")->getCString();
        std::string fullPath;
        bool is_rotated=false;
        std::string writablePath_temp= CCFileUtils::sharedFileUtils()->getWritablePath();
        fullPath = writablePath_temp + CCString::createWithFormat("%s",str_profile_pic.c_str())->getCString();
        CCSprite *profile_pic;
        if (CCFileUtils::sharedFileUtils()->isFileExist(fullPath))
        {
            is_rotated=true;
            profile_pic=CCSprite::create(fullPath.c_str());
            if (profile_pic)
                profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        else
        {
            is_rotated=false;
            fullPath="place holder.png";
            profile_pic=CCSprite::create("place holder.png");
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        CCMask *masked = (CCMask*)cell->getChildByTag(124);
        masked->changeObject(profile_pic);
    }
    return cell;
}
unsigned int SelectPlayerLayer::numberOfCellsInTableView(CCTableView *table)
{
    return existing_users->count();
}
#pragma mark -
#pragma mark ccb delegate methods
#pragma mark -
SEL_CallFuncN SelectPlayerLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "callback_info_show", SelectPlayerLayer::callback_info_show);
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "callback_info_hide", SelectPlayerLayer::callback_info_hide);
    return NULL;
}
SEL_MenuHandler SelectPlayerLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onHomeClicked", SelectPlayerLayer::onHomeClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBackClicked", SelectPlayerLayer::onBackClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onnew_user_clicked", SelectPlayerLayer::onnew_user_clicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onProfilePicClicked", SelectPlayerLayer::onProfilePicClicked);
    return NULL;
}
SEL_CCControlHandler SelectPlayerLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool SelectPlayerLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mAnimationManager", CCBAnimationManager *, this->mAnimationManager);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_title", CCLabelTTF *, this->lbl_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_addnew_user", CCLabelTTF *, this->lbl_addnew_user);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_instr_1", CCLabelTTF *, this->lbl_instr_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_instr_2", CCLabelTTF *, this->lbl_instr_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_select_from", CCLabelTTF *, this->lbl_select_from);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bg_text_field", CCSprite *, this->bg_text_field);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_img_profile", CCSprite *, this->sprt_img_profile);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menu_img_profile", CCMenuItemImage *, this->menu_img_profile);
    return false;
}
bool SelectPlayerLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    //CCLog("onAssignCCBCustomProperty");
    bool bRet = false;
    return bRet;
}