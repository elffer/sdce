##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=v1
ConfigurationName      :=Debug
WorkspacePath          :=/media/elffer/Samsung_T5/projects/autonomous_driving/codes/class_materials/term2/p2_codes
ProjectPath            :=/media/elffer/Samsung_T5/projects/autonomous_driving/codes/class_materials/term2/p2_codes/v1
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=elffer
Date                   :=17/01/19
CodeLitePath           :=/home/elffer/.codelite
LinkerName             :=/usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=/usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="v1.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := /usr/bin/x86_64-linux-gnu-g++
CC       := /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/ukf.cpp$(ObjectSuffix) $(IntermediateDirectory)/tools.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/elffer/Samsung_T5/projects/autonomous_driving/codes/class_materials/term2/p2_codes/v1/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/ukf.cpp$(ObjectSuffix): ukf.cpp $(IntermediateDirectory)/ukf.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/elffer/Samsung_T5/projects/autonomous_driving/codes/class_materials/term2/p2_codes/v1/ukf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ukf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ukf.cpp$(DependSuffix): ukf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ukf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ukf.cpp$(DependSuffix) -MM ukf.cpp

$(IntermediateDirectory)/ukf.cpp$(PreprocessSuffix): ukf.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ukf.cpp$(PreprocessSuffix) ukf.cpp

$(IntermediateDirectory)/tools.cpp$(ObjectSuffix): tools.cpp $(IntermediateDirectory)/tools.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/elffer/Samsung_T5/projects/autonomous_driving/codes/class_materials/term2/p2_codes/v1/tools.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tools.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tools.cpp$(DependSuffix): tools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tools.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/tools.cpp$(DependSuffix) -MM tools.cpp

$(IntermediateDirectory)/tools.cpp$(PreprocessSuffix): tools.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tools.cpp$(PreprocessSuffix) tools.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


