# little_plane  
This game is written totally by c with SDL2.  

I got the idea to write this game from [DungeonRush](https://github.com/Rapiz1/DungeonRush)  

### And the design resources are from [小飞机大战](https://www.taptap.com/app/46852) which belongs to [北京联众互动网络股份有限公司](http://www.lianzhong.com/introduction/)

I used to like this game very much.Howerver it is no longer able to play anymore.  
SDL2 seems old now.But I choose it because it's "pure" C haha.  

虽然不值得一提且有些羞耻，但是环境配置一开始属实难倒我了.  
SDL2属于第三方库，需要自己写makefile。make大概是控制编译的一个说明？其实只是告诉编译器怎么编译而已。对于本简单游戏只用告诉它查找include的地点而已。  
因为SDL2“纯正”的c属性，所以常用的MinGW(Devcpp自带的)编译之完全没有问题，至于我则是使用vscode来做编辑器，因为SDL2的常量、函数实在是太长了www.  

学了一天，做了一天，改了一天bug就写出来的东西，代码其实惨不忍睹。但是写的过程中还是解决了很多问题，比如相机移动的问题、高速物体碰撞检测的问题和AI(暂且称之为AI)的问题等等。  
当然现在还是有很多大大小小的问题的，但是收获已经很大了！  

另外写的时候发现计算图形学也是门有意思的东西haha.