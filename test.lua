print("hello world")

key = ""
function print_dump(table , level)
    level = level or 1
    local indent = ""
    for i = 1, level do
        indent = indent.."  "
    end

    if key ~= "" then
        print(indent..key.." ".."=".." ".."{")
    else
        print(indent .. "{")
    end

    key = ""
    for k,v in pairs(table) do
        if type(v) == "table" then
            key = k
            PrintTable(v, level + 1)
        else
            local content = string.format("%s%s = %s", indent .. "  ",tostring(k), tostring(v))
            print(content)
        end
    end
    print(indent .. "}")
end

local cmotan = require("cmotan")
print(cmotan.version())
local v = { 1.1, 1237981, 2123123, { a = "b", c = "d", e = 4 } , {"what", "is", "wrong"}, "hello world"};
print_dump({cmotan.simple_serialize(v)})
print(#cmotan.simple_serialize(v))