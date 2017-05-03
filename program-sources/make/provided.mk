provided/libprovideddebug.so: provideddebug/CatalogDB.c.o provideddebug/CatalogRecord.c.o provideddebug/Config.c.o provideddebug/CustomerDB.c.o provideddebug/CustomerRecord.c.o provideddebug/Dictionary.c.o provideddebug/Document.c.o provideddebug/DocumentRowList.c.o provideddebug/DocumentUtil.c.o provideddebug/EncryptDecrypt.c.o provideddebug/MyString.c.o provideddebug/OperatorTable.c.o provideddebug/PrintFormat.c.o provideddebug/Registry.c.o provideddebug/UnitTest.c.o
	@mkdir -p provided
	gcc -shared -o provided/libprovideddebug.so provideddebug/CatalogDB.c.o provideddebug/CatalogRecord.c.o provideddebug/Config.c.o provideddebug/CustomerDB.c.o provideddebug/CustomerRecord.c.o provideddebug/Dictionary.c.o provideddebug/Document.c.o provideddebug/DocumentRowList.c.o provideddebug/DocumentUtil.c.o provideddebug/EncryptDecrypt.c.o provideddebug/MyString.c.o provideddebug/OperatorTable.c.o provideddebug/PrintFormat.c.o provideddebug/Registry.c.o provideddebug/UnitTest.c.o

provided/libprovidedrelease.so: providedrelease/CatalogDB.c.o providedrelease/CatalogRecord.c.o providedrelease/Config.c.o providedrelease/CustomerDB.c.o providedrelease/CustomerRecord.c.o providedrelease/Dictionary.c.o providedrelease/Document.c.o providedrelease/DocumentRowList.c.o providedrelease/DocumentUtil.c.o providedrelease/EncryptDecrypt.c.o providedrelease/MyString.c.o providedrelease/OperatorTable.c.o providedrelease/PrintFormat.c.o providedrelease/Registry.c.o providedrelease/UnitTest.c.o
	@mkdir -p provided
	gcc -shared -o provided/libprovidedrelease.so providedrelease/CatalogDB.c.o providedrelease/CatalogRecord.c.o providedrelease/Config.c.o providedrelease/CustomerDB.c.o providedrelease/CustomerRecord.c.o providedrelease/Dictionary.c.o providedrelease/Document.c.o providedrelease/DocumentRowList.c.o providedrelease/DocumentUtil.c.o providedrelease/EncryptDecrypt.c.o providedrelease/MyString.c.o providedrelease/OperatorTable.c.o providedrelease/PrintFormat.c.o providedrelease/Registry.c.o providedrelease/UnitTest.c.o

provided-all: provided/libprovideddebug.so provided/libprovidedrelease.so

provided-clean:
	rm -rf provideddebug providedrelease provided

include make/provided-obj.mk





