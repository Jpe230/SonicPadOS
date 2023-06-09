#
# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This file is executed by build/envsetup.sh, and can use anything
# defined in envsetup.sh.
#
# In particular, you can add lunch options with the add_lunch_combo
# function: add_lunch_combo generic-eng

function mkmpp
{
    local curDir=$(pwd)
    croot
    make package/allwinner/eyesee-mpp/external/compile $@
    make package/allwinner/eyesee-mpp/system/public/compile $@
    make package/allwinner/eyesee-mpp/middleware/compile $@
    make package/install $@
    cd ${curDir}
}

function mkapp
{
    local curDir=$(pwd)
    croot
    make package/allwinner/eyesee-mpp/framework/compile $@
    make package/allwinner/eyesee-mpp/awcdr/compile $@
    make package/install $@
    cd ${curDir}
}

function mkall()
{
    local curDir=$(pwd)
    croot
    make $@
    cd ${curDir}
}

function cleanmpp()
{
    local curDir=$(pwd)
    croot
    make package/allwinner/eyesee-mpp/middleware/clean $@
    make package/allwinner/eyesee-mpp/system/public/clean $@
    make package/allwinner/eyesee-mpp/external/clean $@
    cd ${curDir}
}

function cleanapp
{
    local curDir=$(pwd)
    croot
    make package/allwinner/eyesee-mpp/awcdr/clean $@
    make package/allwinner/eyesee-mpp/framework/clean $@
    cd ${curDir}
}

function cleanall()
{
    local curDir=$(pwd)
    croot
    make package/clean
    make target/clean
    make distclean
    cd ${curDir}
}

cmpp_p () 
{ 
    croot;
    cd package/allwinner/eyesee-mpp/middleware
}
cmpp_s () 
{ 
    croot;
    cd external/eyesee-mpp/middleware/v459
}

capp_p () 
{ 
    croot;
    cd package/allwinner/eyesee-mpp/awcdr
}
capp_s () 
{ 
    croot;
    cd external/eyesee-mpp/awcdr/apps/cdr
}

