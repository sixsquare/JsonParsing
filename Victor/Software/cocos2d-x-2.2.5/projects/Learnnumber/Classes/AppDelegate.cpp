#include "AppDelegate.h"
#include "AppMacros.h"
#include <vector>
#include "SimpleAudioEngine.h"
#include "../Classes/Scene_common.h"
#include "../Classes/SplashLayer.h"
#include "../Classes/SplashLayerLoader.h"
#include "../Classes/ObjCCalls.h"
//#include "../Classes/ChartboostX.h"
//#include "../Ios/RevMob.h"

USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate()
{

}
AppDelegate::~AppDelegate()
{
    
}
bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    pDirector = CCDirector::sharedDirector();
    pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    int count=CCUserDefault::sharedUserDefault()->getIntegerForKey("ads_count");
    count=count+1;
    this->should_display_add();
    CCUserDefault::sharedUserDefault()->setIntegerForKey("ads_count",count);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("should_display_ads",1);
    CCUserDefault::sharedUserDefault()->flush();
    // turn on display FPS
    //pDirector->setDisplayStats(true);
    // set FPS. the default value is 1.0/60 if you don't call this
    this->set_assets_path();
    pDirector->setAnimationInterval(1.0 / 60);
    Scene_common* pScene = new Scene_common();
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    if (pScene)
    {
        pScene->RunWithScene("SplashLayer.ccbi","SplashLayer", SplashLayerLoader::loader(),false,NULL);
        pScene->release();
    }
    return true;
}
void AppDelegate::should_display_add()
{
//    revmob::RevMob *revmob = revmob::RevMob::SharedInstance();
//    
//    revmob->StartSession("534cfbb3349177b8483ce7d0");
    //revmob->SetTestingMode(revmob::kTestingModeWithoutAds);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("should_display_ads",0);
}
//#warning change path for assets based on device resolution..
void AppDelegate::set_assets_path()
{
    //design resolution
    //CCSize designSize = CCSizeMake(480,320);
    //CCSize resourceSize = CCSizeMake(2048,1536);
    //Set the design resolution
    //IMPORTANT LINE!!!(This is the design resolution....)
    CCSize frameSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    // Set the design resolution
    pEGLView->setDesignResolutionSize(frameSize.width, frameSize.height, kResolutionFixedHeight);
    /*
     In this demo, we select resource according to the frame's height.
     If the resource size is different from design resolution size, you need to set contentScaleFactor.
     We use the ratio of resource's height to the height of design resolution,
     this can make sure that the resource's height could fit for the height of design resolution.
     if the frame's height is larger than the height of medium resource size, select large resource.
     */
    float scale_factor;
    std::vector<std::string> searchPaths;
    std::vector<std::string> resDirOrders;
    TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
    if (platform == kTargetIpad || platform ==kTargetIphone)
    {
        searchPaths.push_back("Published-iOS");
        if (frameSize.height >=resources_ipadhd.size.height)
        {
            searchPaths.push_back("resources-ipadhd");
            resDirOrders.push_back("resources-ipadhd");
//            searchPaths.push_back("1");
//            resDirOrders.push_back("1");
//            searchPaths.push_back("2");
//            resDirOrders.push_back("2");
//            searchPaths.push_back("3");
//            resDirOrders.push_back("3");
//            searchPaths.push_back("4");
//            resDirOrders.push_back("4");
//            searchPaths.push_back("5");
//            resDirOrders.push_back("5");
//            searchPaths.push_back("6");
//            resDirOrders.push_back("6");
//            searchPaths.push_back("7");
//            resDirOrders.push_back("7");
//            searchPaths.push_back("8");
//            resDirOrders.push_back("8");
//            searchPaths.push_back("9");
//            resDirOrders.push_back("9");
//            searchPaths.push_back("10");
//            resDirOrders.push_back("10");
            scale_factor=MIN(resources_ipadhd.size.height/frameSize.height, resources_ipadhd.size.width/frameSize.width);
            pDirector->setContentScaleFactor(scale_factor);
        }
        else if (frameSize.height >= resources_ipad.size.height)
        {
            //searchPath.push_back(resources_ipad.directory);
            searchPaths.push_back("resources-ipad");
            resDirOrders.push_back("resources-ipad");
            scale_factor=MIN(resources_ipad.size.height/frameSize.height, resources_ipad.size.width/frameSize.width);
            pDirector->setContentScaleFactor(scale_factor);
        }
        //if the frame's height is larger than the height of small resource size, select medium resource.
        else if (frameSize.height >= resources_iphonehd.size.height)
        {
            if(frameSize.width>=1136)
            {
                    // pEGLView->setDesignResolutionSize(resources_iphonehd.size.width, resources_iphonehd.size.height, kResolutionExactFit);
                searchPaths.push_back("resources-ipadhd");
                resDirOrders.push_back("resources-ipadhd");
//                searchPaths.push_back("resources-iphonehd");
//                resDirOrders.push_back("resources-iphonehd");
                scale_factor=MAX(resources_iphonehd.size.height/resources_iphonehd.size.height, resources_iphonehd.size.width/resources_iphonehd.size.width);
            }
            else
            {
                pEGLView->setDesignResolutionSize(resources_iphonehd.size.width, resources_iphonehd.size.height, kResolutionShowAll);
                searchPaths.push_back("resources-iphonehd");
                resDirOrders.push_back("resources-iphonehd");
                scale_factor=MIN(resources_iphonehd.size.height/frameSize.height, resources_iphonehd.size.width/frameSize.width);
            }
            pDirector->setContentScaleFactor(scale_factor);
        }
        // if the frame's height is smaller than the height of medium resource size, select small resource.
        else
        {
            searchPaths.push_back("resources-iphone");
            resDirOrders.push_back("resources-iphone");
            scale_factor=MIN(resources_iphone.size.height/frameSize.height, resources_iphone.size.width/frameSize.width);
           pDirector->setContentScaleFactor(scale_factor);
        }
    }
    else if (platform == kTargetAndroid || platform == kTargetWindows)
    {
        searchPaths.push_back("Published-Android");
        if (frameSize.height >=resources_ipadhd.size.height)
        {
            searchPaths.push_back("resources-xlarge");
            resDirOrders.push_back("resources-xlarge");
            scale_factor=MIN(resources_ipadhd.size.height/frameSize.height, resources_ipadhd.size.width/frameSize.width);
            pDirector->setContentScaleFactor(scale_factor);
        }
        else if (frameSize.height>=resources_ipad.size.height)
        {
            //CCLog("Large resource are being used");
            searchPaths.push_back("resources-large");
            resDirOrders.push_back("resources-large");
            scale_factor=MIN(resources_ipad.size.height/frameSize.height, resources_ipad.size.width/frameSize.width);
            pDirector->setContentScaleFactor(scale_factor);
        }
        else if (frameSize.height>=resources_iphonehd.size.height)
        {
            //CCLog("resources_iphonehd resource are being used");
            searchPaths.push_back("resources-xlarge");
            resDirOrders.push_back("resources-xlarge");
            scale_factor=MIN(1536/frameSize.height, 2048/frameSize.width);
            pDirector->setContentScaleFactor(scale_factor);
        }
        else if (frameSize.height>resources_Medium.size.height)
        {
            //CCLog("resources_Medium resource are being used");
            searchPaths.push_back("resources-medium");
            resDirOrders.push_back("resources-medium");
            scale_factor=MIN(resources_Medium.size.height/frameSize.height, resources_Medium.size.width/frameSize.width);
            pDirector->setContentScaleFactor(scale_factor);
        }
        else
        {
            //CCLog("resources small  are being used");
            searchPaths.push_back("resources-small");
            resDirOrders.push_back("resources-small");
            scale_factor=MIN(resources_iphone.size.height/frameSize.height, resources_iphone.size.width/frameSize.width);
            pDirector->setContentScaleFactor(scale_factor);
        }
    }
    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(resDirOrders);
    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
}
//This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}