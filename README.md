# The Day of Sagittarius III 复刻版

# 作者
程序：Tom Willow

# 玩法说明

# 下载地址

# 制作初衷

2020年，我看到了《凉宫春日的忧郁》。当看到第27集时，我暂停了长门有希写代码的镜头，当熟悉的Win32 API出现在眼前时，其中又涉及了进程、Hook操作，我突然激动了起来。我从初中开始编程，也曾用Hook试图盗同学qq以进行炫耀，这是我罕见地在屏幕上看到代码，而且也是正确的。我为这细节而感动，所以决定复刻《射手座之日III》这个游戏。

# 用到的库
为了达到和动画相像的效果，我为了适配原生Windows XP做了很多努力。没有使用OpenGL核心模式，只用了OpenGL 1.0的固定管线，也为了操作窗口细节，我没有使用glut库和freeglut库，在OpenGL渲染方面只使用了Win32 API。

除此之外，用到了下列库：
- 文字渲染：FreeType
- NSF解码：nsfplayer

# 参考
LearnOpenGL