void %%FlatStructName%%_delete(%%FlatStructName%%* obj)
{
    free(obj);
}

VALUE %%FlatStructName%%_allocate( VALUE klass )
{
    VALUE obj;
    %%FlatStructName%%* internalObj;

    internalObj = (%%FlatStructName%%*)malloc(sizeof(%%FlatStructName%%));
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - %%FlatStructName%%_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, %%FlatStructName%%_delete, internalObj);
    
    memset(internalObj, 0, sizeof(%%FlatStructName%%));

    return obj;
}
