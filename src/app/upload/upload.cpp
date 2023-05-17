#include "app/upload/upload.h"
#include "ESP32WebServer.h"
ESP32WebServer upload_server(80);
File fsUploadFile;

String getContentType(String filename)
{
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
void handleFileUpload()
{
    HTTPUpload& upload = upload_server.upload();
    
    if(upload.status == UPLOAD_FILE_START)                     // 如果上传状态为UPLOAD_FILE_START
    {
        String filename = upload.filename;                        // 建立字符串变量用于存放上传文件名
        if(!filename.startsWith("/")) 
        {
            if(filename.endsWith(".bin")) filename = "/data/photo/" + filename;
            if(filename.endsWith(".jpg")) filename = "/data/jpg/" + filename;  // 为上传文件名前加上"/"
            if(filename.endsWith(".txt")) filename = "/data/text/" + filename; 
            if(filename.endsWith(".mp3")) filename = "/data/music/" + filename; 
            if(filename.endsWith(".mp4")) filename = "/data/video/" + filename; 
        }
        else
        {
            if(filename.endsWith(".bin")) filename = "/data/photo" + filename;
            if(filename.endsWith(".jpg")) filename = "/data/jpg"+ filename;
            if(filename.endsWith(".txt")) filename = "/data/text" + filename; 
            if(filename.endsWith(".mp3")) filename = "/data/music" + filename; 
            if(filename.endsWith(".mp4")) filename = "/data/video" + filename; 
        }
        Serial.println("File Name: " + filename);                 // 通过串口监视器输出上传文件的名称
        
        fsUploadFile = SD.open(filename, FILE_WRITE,true);            // 在SPIFFS中建立文件用于写入用户上传的文件数据
        
    } 
    else if(upload.status == UPLOAD_FILE_WRITE)          // 如果上传状态为UPLOAD_FILE_WRITE      
    {  
        if(fsUploadFile)
        {
            fsUploadFile.write(upload.buf, upload.currentSize); // 向SPIFFS文件写入浏览器发来的文件数据
            
        }
    } 
    else if(upload.status == UPLOAD_FILE_END)            // 如果上传状态为UPLOAD_FILE_END 
    {
        if(fsUploadFile)                                     // 如果文件成功建立
        {
            fsUploadFile.close();                               // 将文件关闭
            load_photo();
            Serial.println(" Size: "+ upload.totalSize);        // 通过串口监视器输出文件大小
            upload_server.sendHeader("Location","/system/app/upload/html/finish.html");  // 将浏览器跳转到/success.html（成功上传页面）
            upload_server.send(303);                               // 发送相应代码303（重定向到新页面） 
        } 
        else                                               // 如果文件未能成功建立
        {
                Serial.println("File upload failed");               // 通过串口监视器输出报错信息
                upload_server.send(500, "text/plain", "500: couldn't create file"); // 向浏览器发送相应代码500（服务器错误）
        }    
    }
}
bool handleFileRead(String path) 
{
  if (path.endsWith("/"))                    // 如果访问地址以"/"为结尾
  {
    path = "/system/app/upload/html/index.html";                     // 则将访问地址修改为/index.html便于SPIFFS访问
  } 
  
  String contentType = getContentType(path);  // 获取文件类型
  
  if (SD.exists(path))                      // 如果访问的文件可以在SPIFFS中找到
  {                     
    File file = SD.open(path, "r");          // 则尝试打开该文件
    upload_server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}
void respondOK()
{
    upload_server.send(200);
}
void handleUploadRequest()
{
                              
    // 获取用户请求网址信息
    String webAddress = upload_server.uri();
    
    // 通过handleFileRead函数处处理用户访问
    bool fileReadOK = handleFileRead(webAddress);

    // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
    if (!fileReadOK)
    {                                                 
        upload_server.send(404, "text/plain", "404 Not Found"); 
    }
}

void upload_task(void *p)
{
    bool oldMODE=RUNMODE;
    upload_server.on("/system/app/upload/html/upload.html",   // 如果客户端通过upload页面
                    HTTP_POST,        // 向服务器发送文件(请求方法POST)
                    respondOK,        // 则回复状态码 200 给客户端
                    handleFileUpload);// 并且运行处理文件上传函数
    
    upload_server.onNotFound(handleUploadRequest);
    upload_server.begin();
    while(oldMODE==RUNMODE)
    {
        upload_server.handleClient();
        delay(10);
    }
    upload_server.close();
    vTaskDelete(NULL);
}
lv_obj_t*upload_load()
{
    now_screen =lv_obj_create(NULL);
    if(Theme_color==Theme_color_WHITE)
    {
        main_color=lv_color_hex(0xeeeeee);
        text_color=lv_color_hex(0x000000);
    }
    else if(Theme_color==Theme_color_BLACK)
    {
        main_color=lv_color_hex(0x000000);
        text_color=lv_color_hex(0xeeeeee);
    }

    lv_obj_set_style_bg_color(now_screen,main_color, LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    lv_obj_t*upload_ico=lv_img_create(now_screen);
    lv_img_set_src(upload_ico,&upload_icon);
    lv_obj_align_to(upload_ico,now_screen,LV_ALIGN_TOP_MID,0,20);

    lv_obj_t *notice_label=lv_label_create(now_screen);
    lv_obj_set_style_text_font(notice_label,&chinese_hei_16,0);
    lv_obj_set_style_text_color(notice_label,text_color,0);
    lv_label_set_text(notice_label,("请将手机或电脑与此设备\r\n连接到同一WiFi后在\r\n浏览器地址栏搜索:\r\n"+WiFi.localIP().toString()).c_str());
    lv_obj_set_style_text_align(notice_label,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN);
    lv_obj_align(notice_label,LV_ALIGN_CENTER,0,15);

    gesture(0,1,0,1);
    xTaskCreatePinnedToCore(upload_task,NULL,6000,NULL,10,NULL,0);
    return now_screen;
}