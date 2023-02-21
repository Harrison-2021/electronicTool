# 1.项目描述
- 本项目旨在设计一个产品测试工具，根据测试产品的功能模块设计配置文件，根据配置文件设计UI操作界面
- UI界面支持多种输入事件，例如触屏输入，用以人工监测后主动点击确认；网络输入，用以测试程序通过网络传输向主页面发送测试结果
- 系统支持后台命令输出，为系统接收测试结果后的进一步操作提供处理接口
# 2.具体功能模块介绍
## [1. 显示系统设计](https://github.com/Harrison-2021/electronicTool/blob/main/nodes/1.%E6%98%BE%E7%A4%BA%E7%B3%BB%E7%BB%9F%E8%AE%BE%E8%AE%A1.md)
- 基于FrameBuffer编程，使用文件IO库函数操作LCD设备，设计显示器管理层架构，实现不同显示方式。
## [2. 输入系统设计](https://github.com/Harrison-2021/electronicTool/blob/main/nodes/2.%20%E8%BE%93%E5%85%A5%E7%B3%BB%E7%BB%9F%E8%AE%BE%E8%AE%A1.md)
- 基于tslib框架处理触屏输入事件，基于socket编程处理网络输入事件，中间管理层使用多线程编程处理不同输入设备的数据，设计环形缓冲区解决高并发数据传输问题。
## [3.文字系统设计](https://github.com/Harrison-2021/electronicTool/blob/main/nodes/3.%E6%96%87%E5%AD%97%E7%B3%BB%E7%BB%9F%E8%AE%BE%E8%AE%A1.md)
- 基于freetype框架，在LCD屏指定位置显示指定大小的文字
## [4. UI界面设计](https://github.com/Harrison-2021/electronicTool/blob/main/nodes/4.%20UI%E7%95%8C%E9%9D%A2%E8%AE%BE%E8%AE%A1.md)
- 综合应用framebuffer编程和输入系统应用编程，绘制按钮，设计UI界面并相应输入事件
## [5. 页面管理系统与业务综合设计](https://github.com/Harrison-2021/electronicTool/blob/main/nodes/5.%20%E9%A1%B5%E9%9D%A2%E7%AE%A1%E7%90%86%E7%B3%BB%E7%BB%9F%E4%B8%8E%E4%B8%9A%E5%8A%A1%E7%BB%BC%E5%90%88%E8%AE%BE%E8%AE%A1.md)
- 设计页面管理系统框架；使用配置文件封装UI界面参数，读取配置文件数据计算并设计UI界面；主页面支持触屏输入和网络输入等多种输入事件，具有不同响应处理逻辑。
# 3. 项目demo展示
## 3.1 各模块设计demo
### 显示系统和文字设计系统
![在这里插入图片描述](https://img-blog.csdnimg.cn/711f83fe114f48a5a5c2d7d4e04e3c07.png)
### 输入系统
- 触屏输入
![在这里插入图片描述](https://img-blog.csdnimg.cn/a6b977b8f96f40c1bfd0fb39d49e6317.png)
- 网络输入
![在这里插入图片描述](https://img-blog.csdnimg.cn/bfe2be156d564c118429ec8565e616a5.png)
### ui按钮设计
![在这里插入图片描述](https://img-blog.csdnimg.cn/b41f266fd663479a948caffc2ad35a44.png)
### 页面管理系统界面
![在这里插入图片描述](https://img-blog.csdnimg.cn/844742a5a33d45f7bd6ebf5d52de7355.png)
## 3.2 业务系统综合demo
![在这里插入图片描述](https://img-blog.csdnimg.cn/0c0b5c5c1fee4342a021c6a91a129368.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/e1886e4c920b4f468d98a939f50d2eb4.jpeg)

