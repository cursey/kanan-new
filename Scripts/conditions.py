import frida # pip install frida
import sys 
import hexdump # pip install hexdump

session = frida.attach("client.exe")

script = session.create_script("""
// Called when a condition is added (character->condition_mgr->condition->count is increased).
// Notes: args[4] contains information about the condition that is added. After examining the 
// hexdump its clear that the condition ID is stored at args[4] -> offset 0x10.
Interceptor.attach(ptr("0x8043C0"), {
    onEnter: function(args) {
        send("add_condition(" + this.context.ecx.toString() + "): " + args[0].toString() + " " + args[1].toString() + " " + args[2].toString() + " " + args[3].toString() + " " + args[4].toString());
        //send(this.context.ecx.toString());
        var a6 = args[4].readByteArray(0xFF);
        send("", a6);
    }
});
// Called when a condition is removed (character->condition_mgr->condition->count is decreased).
// Notes: args[1] matches args[4] of the above function.
Interceptor.attach(ptr("0x808BF0"), {
    onEnter: function(args) {
        send("remove_condition(" + this.context.ecx.toString() + "): " + args[0].toString() + " " + args[1].toString());
    }
});
""")

def on_message(message, data):
    if len(message) > 0:
        print(message)
    if data:
        hexdump.hexdump(data)

script.on('message', on_message)
script.load()
sys.stdin.read()
