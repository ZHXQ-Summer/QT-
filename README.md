# 来财二手交易平台

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Qt Version](https://img.shields.io/badge/Qt-6.2.0-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20|%20Linux%20|%20macOS-lightgrey.svg)

基于Qt框架开发的C++二手交易平台，集成即时通讯、商品发布、用户社交等核心功能。

## ✨ 核心特性

### 即时通讯系统
- 双人私信聊天室
- 消息历史记录保存
```cpp
// 示例代码片段 - 聊天室创建
ChatRoom* room = ChatManager::getOrCreateRoom(userA, userB);
room->show();
