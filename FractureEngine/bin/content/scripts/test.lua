
LogMessage('Testing from File')

test = {

    Properties = {
        ["MoveSpeed"] = 10,
        ["Jumpheight"] = 5
    }

}
return test


function test:run()

end

function Update()
print('update Function in LUA script')

local vertical = GetAxis("vertical")
local horizontal = GetAxis("horizontal")

print("Vertical: ",vertical)
print("Vertical: ",horizontal)

local translation = vertical - horizontal

print(translation)

end
