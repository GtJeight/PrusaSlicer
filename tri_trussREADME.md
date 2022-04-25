- 修改了PrusaSlicer的部分代码，使其能够读取tri_truss生成的桁架数据，并且在导入stl文件的同时将桁架模型也将桁架一并加入

- 添加类DiffTrussSupport，该类对象的构造函数接收一个路径path，在path下找到存有桁架信息的文件，读入并存储起来；具体实现在PrusaSlicer\src\libslic3r下的DiffTrussSupport.hpp和DiffTrussSupport.cpp中

- 在PrusaSlicer\src\libslic3r的Model.hpp和Model.cpp为ModelObject加入了add_trusssupp函数

  ![image-20220425140624384](C:\Users\GtJeight\AppData\Roaming\Typora\typora-user-images\image-20220425140624384.png)

- 添加桁架结构的实现方式比较粗暴，在Model.cpp中的add_object函数里，我规定读取文件名以下划线开头的stl文件时就执行add_trusssupp函数，加入桁架结构；桁架是由多个柱体网格（杆件）和球体网格（节点）通过平移旋转拉伸构成的，具体见add_trusssupp函数体；这里及下文add_trusssupp函数体涉及到路径的地方都要先作出修改

  ![image-20220425143420159](C:\Users\GtJeight\AppData\Roaming\Typora\typora-user-images\image-20220425143420159.png)

- 改变桁架的杆件形状（如十字柱变圆柱）只需改变参考柱体的模型文件UnitCylinder.stl（见add_trusssupp函数体），参考柱体是半径与高为1mm的模型文件，原点在底面中心；充当节点的参考球体是半径为1mm的球体模型，原点在球心

- 生成PrusaSlicer前要取消DiffTrussSupport.cpp使用的预编译头

  ![image-20220425150110221](C:\Users\GtJeight\AppData\Roaming\Typora\typora-user-images\image-20220425150110221.png)

- 完成生成后运行Prusa，先运行tri_truss算出桁架信息，再于Prusa中导入下划线开头的stl文件，即可获得带有桁架的模型

  ![image-20220425150356637](C:\Users\GtJeight\AppData\Roaming\Typora\typora-user-images\image-20220425150356637.png)

- 添加柱体、球体完毕后，桁架的底端会穿过工作台到下方，由于Prusa的某些代码，它会自动升高模型使得桁架完全处于工作台上方，导致模型主体离开工作台，我目前都是采用手动切割的方法切去桁架底端的凸出部分；下图圈中的小球就是因为桁架底端突出而被垫高脱离工作板的例子

  ![image-20220425151325679](C:\Users\GtJeight\AppData\Roaming\Typora\typora-user-images\image-20220425151325679.png)

