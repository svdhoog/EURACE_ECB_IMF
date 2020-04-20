README file for unittests housing market

This folder contains unittestss for 2 versions.

Settings
========
To switch models, change true/false for the nested models in ECB_Model/ecb_model.xml:

  <model>
   <file>ECB_Model/Housing_Market/decentralized_auction/model.xml</file>
   <enabled>false</enabled>
  </model>
  <model>
   <file>ECB_Model/Housing_Market/centralized_auction/model.xml</file>
   <enabled>true</enabled>
  </model>

Files: compile.bat
==================
Centralized auction:
- compile_unittest_Household_centralized.bat
- compile_unittest_ClearingHouse.bat
- compile_unittest_Bank.bat

Decentralized auction:
- compile_unittest_Household_decentralized.bat
- compile_unittest_House.bat
- compile_unittest_Bank.bat
