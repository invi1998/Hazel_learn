# encoding: utf-8

import os
import subprocess
import platform

# -----------------------------1.检查python环境-----------------------------------
# import pkg_resources

# def install(package):
#     print("Installing {0} module...".format(package))
#     subprocess.check_call(['python', '-m', 'pip', 'install', package])

# def ValidatePackage(package):
#     required = {package}
#     installed = {pkg.key for pkg in pkg_resources.working_set}
#     missing = required - installed

#     # 安装缺失的package
#     if missing:
#         install(package)

# def ValidatePackages():
#     ValidatePackage("requests")
#     ValidatePackage("fake-useragent")


from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
from SetupVulkan import VulkanConfiguration as VulkanRequirements
os.chdir('./../') # Change from devtools/scripts directory to root

premakeInstalled = PremakeRequirements.Validate()
VulkanRequirements.Validate()

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        subprocess.call([os.path.abspath("./scripts/Win-GenProjects.bat"), "nopause"])

    print("\nSetup completed!")
else:
    print("Hazel requires Premake to generate project files.")

