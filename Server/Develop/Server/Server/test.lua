testModel = require "testModel"
local f = testModel.test()
f:TestSetA(100)
print(f:TestGetA())
