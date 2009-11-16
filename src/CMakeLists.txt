
set(BEGET_SRCS HApp.cpp HBeGetSettingView.cpp HCaption.cpp HListItem.cpp
				HListView.cpp HLogView.cpp HMenuItem.cpp HPrefs.cpp
				HSettingWindow.cpp HSoundSettingView.cpp HTaskView.cpp
				HWGetSettingView.cpp HWindow.cpp PasswordWindow.cpp
				SettingsFile.cpp URLSetting.cpp main.cpp HAddUrlDlg.cpp)


add_executable(BeGet ${BEGET_SRCS})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-multichar -Woverloaded-virtual -Wno-unknown-pragmas -Wno-ctor-dtor-privacy")


target_link_libraries(BeGet "be" "game" "media" "tracker" ${LIBSGB} ${LIBTOOLBAR} ${LIBUTILS} ${LIBLOCALE})


#Change the default location of our output
set_property(TARGET BeGet PROPERTY RUNTIME_OUTPUT_DIRECTORY ${BEGET_BINARY_DIR})


haiku_add_resource_def(BeGet ${BEGET_SOURCE_DIR}/resources/BeGet.rdef)


haiku_auto_mimetype(BeGet)
