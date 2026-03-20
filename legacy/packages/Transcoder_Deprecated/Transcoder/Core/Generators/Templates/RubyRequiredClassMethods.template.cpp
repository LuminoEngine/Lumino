static void %%FlatClassName%%_delete(%%WrapStructName%%* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void %%FlatClassName%%_mark(%%WrapStructName%%* obj)
{
	%%MarkExprs%%
}

static VALUE %%FlatClassName%%_allocate(VALUE klass)
{
    VALUE obj;
    %%WrapStructName%%* internalObj;

    internalObj = new %%WrapStructName%%();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - %%FlatClassName%%_allocate");
    obj = Data_Wrap_Struct(klass, %%FlatClassName%%_mark, %%FlatClassName%%_delete, internalObj);

    return obj;
}

static VALUE %%FlatClassName%%_allocateForGetObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    %%WrapStructName%%* internalObj;

    internalObj = new %%WrapStructName%%();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - %%FlatClassName%%_allocate");
    obj = Data_Wrap_Struct(klass, %%FlatClassName%%_mark, %%FlatClassName%%_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}
