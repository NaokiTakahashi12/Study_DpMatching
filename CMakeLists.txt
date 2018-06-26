#
# ccmake and cmake
#
# Author Naoki Takahashi
# Email s16c1077hq@s.chibakoudai.jp
#

message(" ----------------------------------------------------- ")
message("Cmake is ${CMAKE_COMMAND}")
message("
               .
              ,M,
             ,kHW,
            ,kkHfW,
           ,kkkHWfW,
          .kkkkHHffW,  `         `  `  `    `  `  `  `  `  `  `      `         ....
       ` .kkkkkkNVffW,    `  `      .gH@@@HMm` .@@@@b      d@@@@               d@M
    `   .kkkkbkkNVfffW,           (@@#!``_?    d@MH@N    .d@#@@#              .@@F
       .kkkkbkHY4VVVffW,         d@@%         .@@FJ@@.  .d@P,@@%  .dH@@HMa.   J@@! .d@M  .(H@@@N&
      .kkkkbH9~~JVVVVVfW,       .@@F          J@@:,@@; .H@F H@M`  .!   .H@b  .@@@.d@M=   (@M=  .M@b
     .kkkHHD~~~~(HfVVVfVW,      ,@@]         .@@@ .@@].H@F .@@F   .JgQQQH@F  .@@M@@=    ,@@mggggH@#
    .kkHHSzzXUGJ-NVVVVVVfW.  `  .@@M,        .@@   @@h@@t  d@@! .@@Y!  .@@:  d@#.H@h.   d@M???????
   .kHHXzzzzzzzzzXWHWkfVVVW.     (M@@h-...p  W@#   W@@@%  .@@@  (@@&...M@#  .@@% .M@N.  ,@@h....._
  .H8zzzzzuzzuzzzzzzzzXWHWWW.      ?WHHWY   .YYt   ?YYC   ?YY^   ?YHYYTY%  ?YY`  .YY5    7YWHWY
 .")

cmake_minimum_required(VERSION 3.10)

if(NOT PROJECT_NAME)
	get_filename_component(DEFAULT_PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
	set(PROJECT_NAME ${DEFAULT_PROJECT_NAME} CACHE STRING "Project name" FORCE)
endif(NOT PROJECT_NAME)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(${PROJECT_NAME} CXX C)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_VERBOSE_MAKEFILE)
	set(CMAKE_VERBOSE_MAKEFILE false CACHE BOOL "Verbose option" FORCE)
endif(NOT CMAKE_VERBOSE_MAKEFILE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Winline -Wextra -Wall -g -O0")

if(NOT MAIN_FILENAME)
	set(MAIN_FILENAME main CACHE STRING "Main file name" FORCE)
endif(NOT MAIN_FILENAME)

if(NOT USE_GCOV)
	set(USE_GCOV CACHE BOOL "GCOV" FORCE)
endif(NOT USE_GCOV)

if(NOT USE_BOOST)
	set(USE_BOOST false CACHE BOOL "Find and link Boost" FORCE)
endif(NOT USE_BOOST)

if(NOT USE_CV)
	set(USE_CV false CACHE BOOL "Find and link OpenCV" FORCE)
endif(NOT USE_CV)

if(NOT USE_EIGEN3)
	set(USE_EIGEN3 false CACHE BOOL "Find and link Eigen3" FORCE)
endif(NOT USE_EIGEN3)

if(NOT USE_ICE)
	set(USE_ICE false CACHE BOOL "Find and link Zeroc Ice" FORCE)
endif(NOT USE_ICE)

if(NOT USE_ODE)
	set(USE_ODE false CACHE BOOL "Find and link Open Dynamics Engine" FORCE)
endif(NOT USE_ODE)

if(NOT USE_SSH)
	set(USE_SSH false CACHE BOOL "Find and link OpenSSH" FORCE)
endif(NOT USE_SSH)

if(NOT USE_GL)
	set(USE_GL false CACHE BOOL "Find and link OpenGLW" FORCE)
endif(NOT USE_GL)

if(USE_GCOV)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
endif(USE_GCOV)

if(NOT EXECUTABLE_OUTPUT_PATH)
	set(EXECUTABLE_OUTPUT_PATH bin CACHE PATH "Out file insert directory" FORCE)
endif()

if(NOT LIBRARY_OUTPUT_PATH)
	set(LIBRARY_OUTPUT_PATH lib CACHE PATH "Library file insert directory" FORCE)
endif()

if(NOT INCLUDE_DIR)
	set(INCLUDE_DIR include CACHE PATH "Include directory" FORCE)
endif()

if(NOT SOURCE_DIR)
	set(SOURCE_DIR src CACHE PATH "Source directory" FORCE)
endif()

if(NOT EXISTS ${EXECUTABLE_OUTPUT_PATH})
	message(FATAL_ERROR "Not found executable path")
endif()

if(NOT EXISTS ${INCLUDE_DIR})
	message(FATAL_ERROR "Not found include path")
endif()

if(NOT EXISTS ${SOURCE_DIR})
	message(FATAL_ERROR "Not found source path")
endif()

if(USE_GL)
	find_package(OpenGL REQUIRED)

	if(OpenGL_FOUND)
		message(" ---- Found OpenGL ${OpenGL_INCLUDE_DIRS}")
		include_directories(${OpenGL_INCLUDE_DIRS})

	else()
		message(FATAL_ERROR " ---- Not Found OpenGL")

	endif()
	if(APPLE)
		set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${OpenGL_LIBRARIES} glfw "-framework OpenGL")

	elseif(UNIX)
		set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${OpenGL_LIBRARIES} glfw)

	endif()
endif(USE_GL)

if(USE_SSH)
	if(APPLE)
		set(LIBSSH_TOP_DIR /usr/local)

	elseif(UNIX)
		set(LIBSSH_TOP_DIR /usr)

	endif()
	set(libssh_INCLUDE_DIRS ${LIBSSH_TOP_DIR}/include)
	set(libssh_LIBRARIES_DIR ${LIBSSH_TOP_DIR}/lib)

	if(EXISTS ${libssh_INCLUDE_DIRS} AND EXISTS ${libssh_LIBRARIES_DIR})
		message(" ---- Found libssh ${libssh_INCLUDE_DIRS}")
		include_directories(${libssh_INCLUDE_DIRS})
		file(GLOB libssh_LIBRARIES "${libssh_LIBRARIES_DIR}/libssh*.dylib")

	else()
		message(FATAL_ERROR " ---- Not Found libssh Top Directories")

	endif()

	set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${libssh_LIBRARIES})

endif(USE_SSH)


if(USE_ODE)
	message("
	(z1,       .xz<        (O1.       .v<-       .Jvi.      `.z<-       .Jv+.       (Oz-       .zOi.      .Jz<-.     ..zz_.      .&O+-.     .JzOi.
      .I<(k       z><d.      .Sz?}       dI_j.      -K>_>      .qk<_:     .WHZ(<.     dqRz<i     (HHXz1-    .HHHI_(.   .WHWkx_<   .dHHHA+(_   JHHytv!
      (I<(y       O<>z_      .0z<l       fI_<:      XH>~+.     dHk<~<     (HWk<<>    .kWWI<(-   .kkWSI1+.  .bHWYS>_<  .XHWf(k-_< .dWff`jw<~_ dWWK'
      (z>(X`      j<<j`      .kl<z      .bI~(>     .WHI_(:``  .WkW>_(_ `  XkWWz;<.`` XkHt6<<1   dkHP.kz?l  dWHf ,I_.: JXHK  (o.(-JXWK`  jc_(JXXW!
      (z<<X      `(<1z.     `,kz=z      ,U0~(z     .VCk_(iJ@e .MNXI<(l .MHHH@NI(?v+++<1W@H>:+I<+11?W@KI+zI1111zM@K< 1z1111vM@R_ jz1v\   .I__zZw\   `
.......z~(I.......J>~1-......Jkz~z-.....(kw_ 1....HMMMK<.dMHHaMHMNy< d&MMHMHMMk_(1<<~~~?MMI_(z:<:<:?MNZ<(I~:::<?MNc (>:::::J@Nl (>:<1-(JJs> (vO((((((-
(><dH@MZ>.z<<<<d@HMI_(I<<<<d@HNZ:(I<<<<d@@N>.(v<<dHWMM#c.+HX@MMKMMN+.(WMKHMRd@Hc.<_..`..d@R<.1-` `` d@Rz.1__    dMk_ +_````_WMk_ z__+~~H@Mk_.z~~~~~~~~
(++J@@HRz(z(::_J@@Mk+(I(((_Jg@Mko+I____Jg@MI-+I__JHXMMNk-jXX@WMRMMNZ<(dMRHMRd@Nk+z>_____J@HI+z:_____JHHk+w<--.._J@Kl.z--....J@Kl(z-(j--J@@k+(z-----__-
jwwd@HHKwIzOzzzdH@HKlwwzzzzdq@@HuXwzzzzdqH@R?zOzzdHWMMMkzzdWMM#NMMMkOzdMHMHHdHMktOI+++zzdHNIzrI++++1d@NwzuI<<<+1dHNI>Oc<<<++d@HI+zz+dwwdMgRv=zwzzzzzzc
?77777HHZwlC?????HHHIOwC7???WHHHOXzr7?7WWHP1<zv777MMMMM#vzuM#MNMMMMKzldM@MMHW@@RlvwOOwwwd@M0+zXOOwwwd@MIwuXzzzwwd@MI<Owzzzwwd@NI+zwwXWBBBWHz=zVUU9UUU$
     .VWkzOO.   .VWb0zwk.  .yVk>XvzI  .Wyb:.I=wl .WyW{ .OzwWBWMMM'.NOlwH#MHHH@HHOwvWWWHHH@HKzwXWkWHHH@@RvuXkkWkkH@MkzzXkkWkkWHMX1zXK'     XOzz)
    .Z<zYXOzo  .k>jr.ytwl  dSjf (Xvw< JXXV  jOzw.JXWK   Ozwk(WWK`   XZzXWWK` ``(XwrXWW!````(kOwWW:`````jXzXW{``````OIzX\```````WOzI       WXzz[
   .XXx2 (ZXwn.dZXZ` jwXX+JUXK`  4XXk(UXW!  .XuukIOW\   .kZX0zX%    (XuX+vP     wZXZvD      SXWSS      ,WVWW`      jkXX:       WXX0       XXwX[
  (WggY   ?UuXZXW@`   jXXVWX#'    4uXXXU\ `  (XwWXXf     jXXWZf      4uXWk`     ,UZWW{      jXXpr       UXWR       (XXp~       WzXP       d0uX
   ?7!      ?7T7^      .7TY=       ?TT7`      .?TY^      `?7Y=        ?TY!  `    (7Y^      ` ?T=        (7Y^        7T=       `?7Y'       .7T=  `  `
  .....,                              .......                       `          `                       ........
.dMt  ,MN,`                 `          MM]  TMN,    `           `    `              `   . `   .         MM]`_TF `` `  ` `    `     `  ` `  `      ,,HD
jM#    JMN (M](Mm JM%`?] MMp  H!       MM]   dMN.WM/ (D`MMp  M!  JM[  .MM,  dM] (MF .dF` T].M> U_       MM% .x  ?MN, ?#!.+B!_T] vMF zMN..vF qMr?T]
dM#   `(MN ,M].dM ,Mn.p  V,MN.d        MM]   JMN` WN.$  X(MN.d  .%dN. .FWN.((M] .M] MN`    ,MNa,        MMP`TF   F?Mp b dM`   ` ,M[ ,rWN,,] ,Mn.p
,MN,   dMF ,M]`   ,M) !. V  WNk        MM]   dMF   M#   X  WNg .K77Mb .F MN%.M] .M] HN- ` ,.. 7Mb       MM]   (  F ,MNb MM. ?M#`,M[ ,) ?Mg] ,M} =. `
 .T9GJH`'  JMh.   JMh(J8.H.  7B     ` (MMB+zT`!   (ME, .H+  7B.?3..?Ml(E,,9 (Mh.(M5. T8J.dt,8--B'      .MMh-(dM .b, .WE .TN..M@ (ML.Jh. ,M%.(Mh.(@")
	if(APPLE)
		set(ODE_TOP_DIRECTORIES /usr/local/Cellar/ode-drawstuff/0.13/)

	elseif(UNIX)
		set(ODE_TOP_DIRECTORIES /usr/local)

	endif()
	set(ODE_INCLUDE_DIR ${ODE_TOP_DIRECTORIES}/include)
	set(ODE_LIBRARIES_DIR ${ODE_TOP_DIRECTORIES}/lib)

	if(EXISTS ${ODE_INCLUDE_DIR} AND EXISTS ${ODE_LIBRARIES_DIR})
		message(" ---- Found Ode ${ODE_INCLUDE_DIR}")
		include_directories(${ODE_INCLUDE_DIR})
		file(GLOB ODE_LIBRARIES "${ODE_LIBRARIES_DIR}/lib*.a")

	else()
		message(FATAL_ERROR " ---- Not Found Ode Top Directories")

	endif()
	if(APPLE)
		set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${ODE_LIBRARIES} "-framework AGL -framework OpenGL -framework GLUT")

	elseif(UNIX)
		set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${ODE_LIBRARIES} drawstuff GL GLU glut X11 "ode")

	endif()
endif(USE_ODE)

if(USE_CV)
	message("
     `    `    `  `  `    `  ` ` `   `   ` ` ..&QkkA&..`    `    `    `    `    `    `    `    `
  `   `     `      `    `     `          `.dbbbbbbbbbbbWa.     `    `    `    `    `    `    `    `
        `                           `   .dbpbbbbbbbbbbbpbbl
    `      `  `  `   `   `  `  ` `    `.pbpbbbbKY``4bbpbpbbh `   `   `  `   `   `   `  `   `   `
      `                                dpbbppW^      Cbbbpbb;                                     `
  `     `       `  `  `    `        `  bpbbpb\        Wbpbbp$      `       `   `          `   `
     `       `           `    `  `     WbpbbpL       .ppbbpb]  `       `          `  `  `
                  `    `               jbbpbbbh,    .bbpbpbW!    `       `   `                  `
   `   `  `    `    `       `  `        4bbpbbY     .WpbbpW^        `                       `
             `                   `  `    ?Wbp%       .4bbY`    `       `       `  `  `  `     `   `
    `   `        `   `  `  `               (^     `    ``          `       `
                              `  .(+1lz+(..                                                 `
   `  `      `    `           .zllllllllll!     `     .JHe   `   `.dN,`  `   `    `   `   `     `
        `       `   `  `   `.=l=l=l====lz!   `       .HHHHL      .HHHHN,       `
                           .ll==l=lv???<`           JHHHHHHb    .MHHHHHM,
   `  `    `             ` ll=ll=v`             `  .HHHHHHF      (MHHHHHN    `    `   `  `  `  `  `
               `   `  `   .l==l=l~        .......  gHHHHH#        ,HHHHHH.
       `     `    `       .l=l==l-       .l==l=l!  JHHHHHM,    `  JHHHHHM
   `     `                 <l=ll=li.   ..l==l=lv   .MHHHHHMa,  ..dHHHHHHF    `    `  `  `   `   `
                     `      1=l==l===l=l=l=l=lv`    ,MHHHHHHHHHHHHHHHHHF
    `  `     `  `  `   `     ?1l=ll=l==l==llz!       .THHHHHHHHHHHHHHM=        `
                               _<1ll=ll=lz<!            7WHHHHHHHHMY^        `    `   `   `   `   `
             `              ......           `            ....., ..    ..,
                    `      MM`77TMb        `      `      JM`77TM]JM|   M#             `   `   `
         `       `        .MF    M#`MNT`Mb .M``M[ MNT`Mp`M#    `` MN. .M%         `
     `                    .MF    M# MF  (M MNkQM@ MF  J# M#    ., ,Mb.MF
                      `    MNJ..(MF MN..J# HN..gK MF  J# dM&...MF  dMdM`       `                `
             `  `            ???!  `MF.?!   -??`  ?!  .!   ???!`    ??!                 `   `
    `  `           `                HD")
	find_package(OpenCV REQUIRED)

	if(OpenCV_FOUND)
		message(" ---- Found OpenCV ${OpenCV_INCLUDE_DIRS}")
		include_directories(${OpenCV_INCLUDE_DIRS})

	else(OpenCV_FOUND)
		message(FATAL_ERROR " ---- Not Found OpenCV")

	endif(OpenCV_FOUND)

	set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${OpenCV_LIBRARIES})

endif(USE_CV)

if(USE_BOOST)
	message("
               ` ` `` `` `  `
            ` ` ```` ```` `` `
          ` ````````````````````` `             (+++J                                                                             .+++J,
         ` ` ``````````````````` `` ` `        .MMMMF                                                                             -MMM#`
        ` ```````.zll===1- .```````` ` ` `     .MMMM\                                                                             dMMMD
        ` ```` ..ztllll=v!`...````````` `      dMMMMJ(((((-.,           ...((-...            ....((-...        ..(((((((((((, .((JMMMMa(((,
      `  ` `` .(Orttttlv~. .......````` ` `   .MMMMMMMMMMMMMMN,     .(gMMMMMMMMMMMm,      .jMMMMMMMMMMMNa.   .MMMMMMMMMMMMMM% (MMMMMMMMMMM}
       ` ````..(Orrrttv_- (==????>-``````  `  .MMMMY????7TMMMMN<  .jMMMMM9`??`WMMMMN[   .dMMMMM`7??TMMMMMN- .MMMM=`           ???MMMM@????
      ` `````.._(7711>__.Jl====???>-`.````   `dMMM#`      JMMMM] .dMMMM%       ?MMMM#  .MMMM#'       HMMMMD JMMMN,......        .MMMM}
     ` ` ```.-......._~.jtlll===????_```` `   MMMMD       .MMMM] JMMMMF        .MMMM# .MMMMM!        (MMMM$ (MMMMMMMMMMMMNe.    +MMM#`
     ` `````._(wwwwOO+._.1tlll====?>_.```  ` .MMMM\       JMMM#! MMMMN:        (MMMMt .MMMM#        .dMMMM!   ?```````HMMMMb    dMMMF
    ` ` ``` _(uuuuzzzvo._ 1ttlll==> .````    gMMM#`     .dMMMM\  MMMMML       .MMMMF  .MMMMM,      .dMMMM%             MMMM@   .MMMM}
     ` ``` _JZZZuuuuzzvI-_.?<<<<<!`.``` ``  .MMMMNNNNNNMMMMM#'   (MMMMMNJ-.(&MMMMM=    TMMMMMNJ..(gMMMM#^  .(((((((((JMMMM#!   (MMMMNgJJ
     `` `` _4ZZZZuuuuuzZ! .....`.`````      (MMMMMMMMMMMMM9^      .TMMMMMMMMMMMB=       ?WMMMMMMMMMMM9!   .MMMMMMMMMMMMMM`      7MMMMMM@
      ````` -zZZZZZuuuZ!...````````` ` `                              _?777?`               ??777!`
    `   ```` .jUUX0XXC~.````````` `     `
     `   ```` ........_```````       `      J=?    ..+     .o.          .}      d!    Rz0`   .wOX\    .U|    .KOW:   .K    .kO>    .ROv`
      `  ` ````````````````                 C+<     ?`     ,!           ?O+    .$    .ZJ=    ,>(6    ,=75    (!(t    ,t    ,S&_    .+JY
               `` ` ` `       `
         `  `      `   `   `")
	find_package(Boost	REQUIRED COMPONENTS thread system)

	if(Boost_FOUND)
		message(" ---- Found Boost ${Boost_INCLUDE_DIRS}")
		include_directories(${Boost_INCLUDE_DIRS})

	else(Boost_FOUND)
		message(FATAL_ERROR " ---- Not Found Boost")

	endif(Boost_FOUND)

	set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${Boost_LIBRARIES} Boost::thread)

endif(USE_BOOST)

if(USE_EIGEN3)
	message("
	                            jma-..
                                   (WggHkmx
            ...                     ?HHHHgH;
            ?WHkn-...-+&sAA&+J.....JdMMHkH@R-                `    `
             (HMHHHgHqWkXkXXXWHHqHM#MMMMMWWqs. `  `  `  `  `    `
   .(<        ?WMMHHHHBH@HpWWgHkbbHMMHMMHWwWHo                      `
  jXy~         j@HpWC`  ?HHC!?THmWWMMHHMNkH@MN+         `    `
 .(dHn.   `   (HHkpk_ `(zXSz_`(dWWWHHHMMMMHHM##-  `  `    ` aaaaaaaaaaaaaaa-  .AAA   `   `   `   `   `   `   `   `   `   `   `   `   `
  dWHmMo     .dggmHk+.(ZXHHX-.jyyWHMH?WMMNMH##MR.           pppUUUUUUUUUUUU:  ,WWW                                                      `
  jHHHHWXx    vHgHkWWHHM0jXkHHpWWHMH0` ???TMMH9W}  `  `  `  ppp`
   ?MHHHWHn.. (W@HHgHkkVkdHHWHHHHMMC`          (:           ppp`              .JJJ     ` .(dXWka..JJ,  `  ` ..dXWkA&,   `  (JJ, .(dXWk+,
    ?HgHMHHHWXQWMMgHHHWUXHXXWHMMMMMma.        .c`       `   ppp`              .ppR     .pppVY``TWpppP     .ppXY`77TWpp+    dppAppUYT4Wpph
     (WgMMMqqqqkbkbWWXrO=zOXyWpkkkqqmgH+.  `  (>`  ` `      pppWXXXXXXXXXX)   .ppR    JppY       .ppP    dpW^       jpp[   dppf^      4pp[
       vHgggmHqkkkW00Ol=??zOwXpppfpfWHHHm-    (~         `  ppfYTTTTTTTTTT    .ppR   .ppf        .ppP   ,pp          ppW   dpp}       ,pfR
        jHHHmgHHHHWXI=l=?<<+zXWWWHNHkWkpHHQe..z+.     `     ppp`              .ppR   dpp         .ppP   Xpppppppppppppfp   dpp}       .ppR
       .dHH@HHWWWSXXv===?;::;vUUWHHMMMMHHWHMmdkX[   `       ppp`              .ppR   dpp)        .ppP   Wpp:````````````   dpp}       .fpR
       jH@HHWZuzZZz1?zI=?<;;;zUZwW@MMMMMHHHM@MH9{       `   ppp`   `          .ppR   (pph        .ppP   zpp[               dpp}       .ppR
     `.X@MmqpWktXZ???w0?>>;>>+zOdWMHHMMMNHMH@MR-          ` ppp`     `   `    .ppR    4ppn.     .JppP   .Wpp+         .,   dpp}       .ppR
     `(W@@gHHWXwWOl==???zy>>>?zXwMNmwWH@MMMHMHkH+    `      pppAAAAAAAAAAAA-  .pfR     TppppbbppW4ppP     4pppn(....dpp$   dpp}       .pfR
      (WHgggmHSwW0tz<>>?1I=1z=zZuXMMHXUHHqkqHHHY!       `   TTTTTTTTTTTTTTT:  .TTY       ?7```7` .pfP       ?TUWpppWY`=`   ?TT:       .TT3
     `(dHHmmHWurWkOz<>?===lwpwwWkWHMMNkdHHHHMMR-                                                 (pp
       (XW@gHWWXXHWwzzOOOOwwuXWHbHH@MNNmdY3?UWk{   `                                   A-...  ..dpp%
        (WHmkHkHWHHWWWkWWWWyXWWHH@MHH@MMS_ -XHD`      `           `                    WppppppppWY!
         ~?zXH@@M@HHHgmkkHHHHHHHHMMMMMMD! .HHN,         `    `                             ```
           ?7CVU0YTUU9UWHHHHMHHHWUXYT!`    ?WHHH>  `
                       `_?<!`` _!             ?!`          `    `  `")
	if(APPLE)
		find_package(Eigen3	REQUIRED COMPONENTS Dense Core Geometry)

		if(Eigen3_FOUND)
			message(" ---- Found Eigen3 ${EIGEN3_INCLUDE_DIRS}")
			include_directories(${EIGEN3_INCLUDE_DIRS})

		else(Eigen3_FOUND)
			message(FATAL_ERROR " ---- Not Found Eigen3")

		endif(Eigen3_FOUND)
	elseif(UNIX)
		if(EXISTS $ENV{EIGEN3_INCLUDE_DIR})
			message(" ---- Used Unix Eigen3 Dir $ENV{EIGEN3_INCLUDE_DIR}")
			include_directories($ENV{EIGEN3_INCLUDE_DIR})

		else()
			message(" ---- Not Defind of EIGEN3_INCLUDE_DIRS")

		endif()
	endif()
endif(USE_EIGEN3)

if(USE_ICE)
	message("
	            ..(zzzzzzzzzzw&(..
              .(wzzzzzzzzzzzzzzzzvvvzw+.
           ..wzzzzzzzzzzzzzzzzzw77!`   `~`
         .JzzzzzzzzzzzzzvzzvzZ^    ....
        (zzvvzzzzzzzzzzzzzzzZ`   (zzvvvzn.  ..
      .wzzzzzzzzzzzzzzzzzzzz!   Jzzzvvzzzzzzzzn.  `
     .vvzzzzzzzzzzzzzzzzvzzZ   .zzzzzzzzzzzzzzzw.    `  ..............                                                       ...(((...
    .zzzzzzzvzzzzzzzzzzzzzvw   .zvzzzzzzzzzzzzzzw.      MMMMMMMMMMMMMM                                                    .+MMMMMMMMMMM^
    Jvzzzzzzzzzzzzzzzzzzzvvv-   jzzzvzzzz7?77Ozzzl               .MMM'                                                   dMM#^        `
   .zvzzvzzzzzzzzzzzzzzzzzvvw.   ?OzzzzZ!   .zzzzz_             .MM@        .JMMMNg..     .gp ..gMM#    ..gMMMNJ.       dMM=
   Jvvzzzzzvzzvzzzzzzzzzzzzvvw.            .wzzzzz)            (MM$       .MM#`777WMMe    .MM.MM`7`=  .MMM`777`MMN,    .MMF
   wzzzzzzzzzzzzzzzzzzzzzzzzvvvzo........zzzzzzzzzI          .dMM^       .MM$      (MM[   .MMM'      .MMF       ?MM]   dMM~
   wzzzzvzzzzzzzzzzzzzzzvzzzzzvzzzzzzzzvvvzzzzzzvzI         .MM#`        dMN........MMF   .MM        (MM`        MMN   dMM~
   jzzzzzzzvzzvzzzzzzzzzzzzzzzzzzzzzzzzzzvzzzzzzzv}        .MMD          MMMMMMMMMMMMMD   .MM}       MMM         MMM   JMM]
   .zvzzzzzzzzzzzzzzzzzzzzzvzzzzzzzzzzzzzzzzvzzzzz:      .JMM^           dMN.             .MM}       (MM,        MM#   .MMN,
    zzvzzzzzzzzzzzzzzzzzvzzzzzzzzzzzzzzzzzzzzzzzzI      .MM#`            .MMN.            .MM}        MMN,     .(MM^    .MMMe.
    .zzzzvzzvzzzzzzzzzzzzzzzzzzzzzzzzzzzvzzzzzzzw`   ` .MMMMMMMMMMMMMMF    TMMNa++gNMM_   .MM}         TMMNg++dMM#'       ?MMMMNNgNNMMF
     .vvzzzzzzzzzzzzzzzzzzzvzzzzzzzvzzzzzzzvzzvw'      .``````````````^      _`T````!     .``!           .`````^             ?`T`````^
      .OzvzzzzzzzzzzzzzzvzzzzzzzzzzzzzzzzzzzzzZ!
        jzzzvzzzzzzzzzzzzzzzzzzzzzzzzzvzzzzzz7
         .1zzzzzzzzzzzzzzzzvzzzzzzzvzzzzzvzC`      `
           .?wzvzzzzzzzzvzzzzzzzzzzzzzzzw7`
              _7wzvzzzzzzzzzzzzzzzzzzZ7!        `
                  _?7CzzzzzzzzzvC77`                `")
	find_package(Ice REQUIRED COMPONENTS Ice++11 IceBox++11 IceSSL++11 IceGrid++11 IceDiscovery++11)

	if(Ice_FOUND)
		message(" ---- Found Ice ${Ice_INCLUDE_DIRS}")
		include_directories(${Ice_INCLUDE_DIRS})
		add_definitions("-D ICE_CPP11_MAPPING")
		
		if(NOT ICE_DIR)
			set(ICE_DIR ice CACHE PATH "find .ice file" FORCE)
		endif()

		if(EXISTS ${ICE_DIR})
			file(GLOB ICE_FILES "${ICE_DIR}/*.ice")
			set(ICE_OUTPUT_DIRECTORY ice_create_files CACHE PATH "Ice create file out-put directory" FORCE)
			
			if(NOT EXISTS ${Ice_SLICE2CPP_EXECUTABLE})
				message(FATAL_ERROR " ---- Not found ice slice")
			endif()
			if(NOT DEFINED ICE_FILES)
				message(FATAL_ERROR " ---- Not found ice files")
			endif()
			if(NOT EXISTS ${ICE_OUTPUT_DIRECTORY})
				message(FATAL_ERROR " ---- Not found ice output directory")
			endif()

			get_filename_component(ICE_OUTPUT_DIRECTORY ${ICE_OUTPUT_DIRECTORY} ABSOLUTE)

			foreach(ICE_FILE ${ICE_FILES})
				get_filename_component(ICE_CREATE_FILENAME ${ICE_FILE} NAME_WE)
				set(ICE_OUTPUT_FILES ${ICE_OUTPUT_FILES} ${ICE_OUTPUT_DIRECTORY}/${ICE_CREATE_FILENAME}.cpp ${ICE_OUTPUT_DIRECTORY}/${ICE_CREATE_FILENAME}.h)
			endforeach()

			add_custom_command(OUTPUT ${ICE_OUTPUT_FILES} DEPENDS ${ICE_FILES} COMMAND "${Ice_SLICE2CPP_EXECUTABLE}" ARGS ${ICE_FILES} --output-dir ${ICE_OUTPUT_DIRECTORY})

			foreach(ICE_OUTPUT_FILE ${ICE_OUTPUT_FILES})
				if("${ICE_OUTPUT_FILE}" MATCHES ".cpp")
					set(SOURCE_FILES ${SOURCE_FILES} ${ICE_OUTPUT_FILE})
				endif()
			endforeach()

			set(INCLUDE_DIRS ${INCLUDE_DIRS} ${ICE_OUTPUT_DIRECTORY})

		else()
			message(FATAL_ERROR " ---- Not found ice directory")

		endif()
	else(Ice_FOUND)
		message(FATAL_ERROR " ---- Not Found Ice")

	endif(Ice_FOUND)

	set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${Ice_LIBRARIES})

endif(USE_ICE)

file(GLOB CXX_SOURCE_FILES "${SOURCE_DIR}/*.cpp")
file(GLOB C_SOURCE_FILES "${SOURCE_DIR}/*.c")
set(SOURCE_FILES ${SOURCE_FILES} ${CXX_SOURCE_FILES} ${C_SOURCE_FILES})

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${SOURCE_DIR} ${INCLUDE_DIR})
include_directories(${INCLUDE_DIRS})

set(OUT_TARGET_NAME ${PROJECT_NAME}.out)

foreach(SOURCE_FILE ${SOURCE_FILES})
	if(NOT "${SOURCE_FILE}" MATCHES "${MAIN_FILENAME}")
		get_filename_component(LIBRARY_FILENAME ${SOURCE_FILE} NAME_WE)

		if("${SOURCE_FILE}" MATCHES ".cpp" OR "${SOURCE_FILE}" MATCHES ".c")
			add_library(${LIBRARY_FILENAME} ${SOURCE_FILE})
			set(LIBRARY_FILES ${LIBRARY_FILES} ${LIBRARY_FILENAME})

		endif()
	else()
		set(MAIN_SOURCE_FILE ${SOURCE_FILE})

	endif()
endforeach()

add_executable(${OUT_TARGET_NAME} ${MAIN_SOURCE_FILE})

if(LIBRARY_FILES)
	add_dependencies(${OUT_TARGET_NAME} ${LIBRARY_FILES})
endif()

set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${LIBRARY_FILES})

set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} stdc++ pthread m)
set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} python2.7)

target_link_libraries(${OUT_TARGET_NAME} ${ALL_LINK_LIBRARIES})

message(" ----------------------------------------------------- ")

