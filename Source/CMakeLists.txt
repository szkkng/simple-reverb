# ==============================================================================
#
#  Copyright 2021, 2022 Suzuki Kengo
# 
#  Simple Reverb is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
# 
#  Simple Reverb is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with Simple Reverb. If not, see <http://www.gnu.org/licenses/>.
# 
# ==============================================================================

target_sources("${PROJECT_NAME}"
    PRIVATE
        EditorContent.cpp
        SimpleReverbEditor.cpp
        SimpleReverbProcessor.cpp)

add_subdirectory(GUI)
