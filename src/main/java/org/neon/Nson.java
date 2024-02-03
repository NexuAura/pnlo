package org.neon;

import java.util.HashMap;
import java.util.Map;

public class Nson {
    private Map<Object, Object> Value = new HashMap<>();
    private void put(Object key, Object value) { Value.put(key, value); }
    private void set(Object key, Object value) { Value.put(key, value); }
    private Object get(String name) { return Value.get(name); }

    String getString(String name) { return (String)Value.get(name); }
    int getInt(String name) { return (int) Value.get(name); }
    boolean getBool(String name) { return (boolean) Value.get(name); }
    float getFloat(String name) { return (Float) Value.get(name); }
    double getDouble(String name) { return (double) Value.get(name); }

    String str(){
        String ss = "";
        // 直接使用foreach循环遍历Map
        for (Map.Entry<Object, Object> entry : Value.entrySet()) {
            ss += "\"" + entry.getKey() + "\"" +
                    ":" +
                    "\"" + entry.getValue() + "\""
            +"; ";
        }
        return ss;
    }

    public static void main(String[] args) {
        Nson Obj = new Nson();
        Obj.set("text1", "e1");
        Obj.set("text2", "e2");
        System.out.printf("t:" + Obj.get("text1")+"\n");
        System.out.printf(Obj.str());
    }

}