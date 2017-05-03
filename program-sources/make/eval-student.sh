#!/bin/bash

CATALOGDB_LIST="CatalogDB_appendRecord CatalogDB_close CatalogDB_create CatalogDB_getRecordCount CatalogDB_insertRecord CatalogDB_open CatalogDB_openOrCreate CatalogDB_readRecord CatalogDB_removeRecord CatalogDB_writeRecord"
CATALOGRECORD_LIST="CatalogRecord_finalize CatalogRecord_getValue_basePrice CatalogRecord_getValue_code CatalogRecord_getValue_designation CatalogRecord_getValue_rateOfVAT CatalogRecord_getValue_sellingPrice CatalogRecord_getValue_unity CatalogRecord_init CatalogRecord_isValueValid_code CatalogRecord_isValueValid_positiveNumber CatalogRecord_read CatalogRecord_setValue_basePrice CatalogRecord_setValue_code CatalogRecord_setValue_designation CatalogRecord_setValue_rateOfVAT CatalogRecord_setValue_sellingPrice CatalogRecord_setValue_unity CatalogRecord_write"
CUSTOMERDB_LIST="CustomerDB_appendRecord CustomerDB_close CustomerDB_create CustomerDB_getRecordCount CustomerDB_insertRecord CustomerDB_open CustomerDB_openOrCreate CustomerDB_readRecord CustomerDB_removeRecord CustomerDB_writeRecord"
CUSTOMERRECORD_LIST="CustomerRecord_finalize CustomerRecord_getValue_address CustomerRecord_getValue_name CustomerRecord_getValue_postalCode CustomerRecord_getValue_town CustomerRecord_init CustomerRecord_read CustomerRecord_setValue_address CustomerRecord_setValue_name CustomerRecord_setValue_postalCode CustomerRecord_setValue_town CustomerRecord_write"
DICTIONARY_LIST="Dictionary_create Dictionary_destroy Dictionary_format Dictionary_getEntry Dictionary_setNumberEntry Dictionary_setStringEntry"
DOCUMENT_LIST="Document_finalize Document_init Document_loadFromFile Document_saveToFile"
DOCUMENTROWLIST_LIST="DocumentRow_create DocumentRow_destroy DocumentRow_finalize DocumentRow_init DocumentRowList_finalize DocumentRowList_get DocumentRowList_getRowCount DocumentRowList_init DocumentRowList_insertAfter DocumentRowList_insertBefore DocumentRowList_pushBack DocumentRowList_removeRow DocumentRow_readRow DocumentRow_writeRow"
DOCUMENTUTIL_LIST="computeDocumentNumber formatDate readString writeString"
ENCRYPTDECRYPT_LIST="decrypt encrypt"
MYSTRING_LIST="compareString concatenateString copyString copyStringWithLength duplicateString icaseCompareString icaseEndWith icaseStartWith indexOfChar indexOfString insertString makeLowerCaseString makeUpperCaseString stringLength subString toLowerChar toUpperChar"
OPERATORTABLE_LIST="OperatorTable_create OperatorTable_destroy OperatorTable_findOperator OperatorTable_getName OperatorTable_getPassword OperatorTable_getRecordCount OperatorTable_loadFromFile OperatorTable_removeRecord OperatorTable_saveToFile OperatorTable_setOperator"
PRINTFORMAT_LIST="PrintFormat_finalize PrintFormat_init PrintFormat_loadFromFile"

ALL_LIST="${MYSTRING_LIST} ${ENCRYPTDECRYPT_LIST} ${OPERATORTABLE_LIST} ${CATALOGRECORD_LIST} ${CATALOGDB_LIST} ${CUSTOMERRECORD_LIST} ${CUSTOMERDB_LIST} ${DOCUMENTUTIL_LIST} ${DOCUMENTROWLIST_LIST} ${DOCUMENT_LIST}  ${PRINTFORMAT_LIST} ${DICTIONARY_LIST}"

MODULES_LIST="MyString EncryptDecrypt OperatorTable CatalogRecord CatalogDB CustomerRecord CustomerDB DocumentUtil DocumentRowList Document PrintFormat Dictionary"




testModule() {
    m=$1
    param=""
    for p in $(echo $MODULES_LIST | sed "s/$m//g"); do
        param="$param disable-unit-$p"
    done

    result=$(debug/facturation disable-gui auto-eval silent-tests synthetic-registry disable-debugtrap $param 2>&1 | grep "##AUISFGWLOSP REGISTRY DONE PSOLWGFSIUA##")
    echo -n "#"
    if [[ -n $result ]]; then
        echo -n "success;" > results/testmod-$m.txt
    else
        echo -n "fail;" > results/testmod-$m.txt
        #echo $result;
        #debug/facturation disable-gui auto-eval silent-tests synthetic-registry $param 2>&1
        #echo "debug/facturation disable-gui auto-eval silent-tests synthetic-registry $param"
    fi
}

doPrepare() {
    echo "  -> Extracting sources..."
    tar -xzf $archive -C $dir --strip-components 1

    echo "  -> Copying original sources..."
    for x in App.c Bill.c Catalog.c CatalogDBUnit.c CatalogRecordUnit.c CatalogRecordEditor.c CustomerRecordEditor.c Customer.c CustomerDBUnit.c CustomerRecordUnit.c DictionaryUnit.c DocumentEditor.c DocumentRowListUnit.c DocumentUnit.c DocumentUtilUnit.c EncryptDecryptUnit.c GtkCatalogModel.c GtkCustomerModel.c main.c MainWindow.c MyStringUnit.c Operator.c OperatorTableUnit.c Print.c PrintFormatUnit.c Quotation.c; do
	cp ../src/$x $dir/src/$x
    done
    cp -r ../provided $dir/
    cp -r ../make $dir/
    cp -r ../include $dir/
    cp -r ../printformat $dir/
    echo "include make/main.mk" > $dir/Makefile
    echo "include make/main-obj.mk" >> $dir/Makefile
    echo "include make/valgrind.mk" >> $dir/Makefile

    echo -n "$lastname;$firstname;" > $dir/results/result.txt

}

doCompile() {
    echo "  -> Compiling sources... "
    if (make -j8 all 2> /dev/null > /dev/null) ; then
	echo -n "compile;" >> results/result.txt
    else
	echo -n "fail;" >> results/result.txt
	# recompile to do tests without some warnings errors
	echo "Compilation failed. Recompiling with lazy flags..."
	EXTRA_WARNINGS="-Wno-error=conversion -Wno-error=format -Wno-error=implicit-function-declaration -Wno-error=unused-but-set-variable" make -j8 all > /dev/null
    fi        
}

doFunctions() {
    echo -n  "  -> Testing implementation functions... "    
    cat $(grep IMPLEMENT src/*.c | cut -f1 -d ':') \
    | sed 's/\(.*\)IMPLEMENT(\(.*\))(/\1user_\2(/g' \
    | sed "s/UNUSED(\([^)]*\))/\1/g" \
    | sed "s/BASEPATH//g" | sed "s/\/\*/\n\/*/g" | sed 's/\/\*@unused@\*\/ x//g' | sed "s/\(safeFileRead.*\)/\1;/g" | sed "s/\(safeMalloc.*\)/\1;/g" \
    | sed 's/||\\/||/' \
    | java -Dfile.encoding=UTF-8 -jar ../../../analyzer/target/analyzer-simple.jar \
    | while read line; do 
        fct=$(echo $line | cut -f3 -d '#' | sed "s/user_//g")
        if [[ -n $(echo $line | grep "provided_$fct") ]]; then
            echo "$fct";
        fi
    done | sort | uniq > results/notimplemented.txt 
    
    for f in ${ALL_LIST}; do
	if [[ -n "$(cat results/notimplemented.txt| grep $f)" ]]; then
	    echo -n "provided;" >> results/result.txt
	else
	    echo -n "user;" >> results/result.txt
	fi
    done
    echo ""
}

doModules() {
    echo -n "  -> Testing modules by modules... "   
    for m in ${MODULES_LIST}; do
	testModule $m &
    done
    wait
    for m in ${MODULES_LIST}; do
        cat results/testmod-$m.txt >> results/result.txt
    done
    echo ""
}

doAllInOne() {
    echo "  -> Testing all in one..."
    result=$(debug/facturation disable-gui auto-eval silent-tests synthetic-registry disable-debugtrap 2>&1 | grep "##AUISFGWLOSP REGISTRY DONE PSOLWGFSIUA##")
    if [[ -n $result ]]; then
	echo -n "success;" >> results/result.txt
    else
	echo -n "fail;" >> results/result.txt
    fi
    result=$(release/facturation disable-gui auto-eval silent-tests synthetic-registry disable-debugtrap 2>&1 | grep "##AUISFGWLOSP REGISTRY DONE PSOLWGFSIUA##")
    if [[ -n $result ]]; then
	echo -n "success;" >> results/result.txt
    else
	echo -n "fail;" >> results/result.txt
    fi
}

WIDELINE="\n                                                                                                                                                                 \n"

doPreValgrind() {
    echo "  -> Getting valgrind logs..."
    rm results/valgrind.txt 2> /dev/null >/dev/null
    (make VALGRIND=auto-eval valgrind-debug-stripped | grep -v "##AUISWLOSP" >> results/valgrind.txt) || (echo "ERREUR VALGRIND" >> results/valgrind.txt)
}

doValgrind() {
    if [[ -n "$(grep SIGSEGV results/valgrind.txt)" ]]; then
	echo -n "0;" >> results/result.txt
    else
        kdialog --title "valgrind $lastname $firstname" --yesnocancel "$(<results/valgrind.txt)$WIDELINE" --yes-label "0 Nul" --no-label "1 Moyen" --cancel-label "2 Bien"
	echo -n "$?;" >> results/result.txt
    fi
}

doPreVariables() {
echo "  -> Extracting variables..."
    cat $(grep IMPLEMENT src/* | cut -f1 -d ':') \
	| sed 's/\(.*\)IMPLEMENT(\(.*\))(/\1user_\2(/g' \
	| sed "s/UNUSED(\([^)]*\))/\1/g" \
	| sed "s/BASEPATH//g" \
	| sed "s/\/\*/\n\/*/g" \
	| sed 's/\/\*@unused@\*\/ x//g' \
	| sed "s/\(safeFileRead.*\)/\1;/g" \
	| sed "s/\(safeMalloc.*\)/\1;/g" \
	| java -Dfile.encoding=UTF-8 -jar ../../../analyzer/target/analyzer-complete.jar \
	| grep -v "1       const char * CATALOGDB_FILENAME" \
	| grep -v "1       char * content" \
	| grep -v "1       CustomerRecord_FieldProperties properties" \
	| grep -v "1       const char * CUSTOMERDB_FILENAME" \
	| grep -v "1       CatalogRecord_FieldProperties properties" \
	> results/variables.txt
}

doVariables() {
    kdialog --title "Nommage des variables $lastname $firstname" --yesnocancel "$(<results/variables.txt)$WIDELINE" --yes-label "0 Nul" --no-label "1 Moyen" --cancel-label "2 Bien"
    echo -n "$?;" >> results/result.txt
}

doEnd() {
    echo "" >> results/result.txt
}

doAnalyzer() {
    echo "Building analyzer..."
    cd  ../analyzer
    mvn clean install 2> /dev/null > /dev/null
    cd -
}

#echo ${ALL_LIST} ${MODULES_LIST}

# prepare : build archive list and compile programs


case "$1" in

    prepare)
    
	doAnalyzer

	cd students

	for archive in *.tar.gz; do
	    export dir=$(echo $archive | sed "s/\.tar\.gz//g")
	    rm -rf $dir 2> /dev/null > /dev/null
	    mkdir -p $dir $dir/results
	    
	    export firstname=$(echo $dir | cut -f 2 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
	    export lastname=$(echo $dir | cut -f 1 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
	    
	    echo "Preparing processing of $firstname $lastname..."
	    echo "$firstname;$lastname" >> ../list.txt
	    doPrepare
	    cd $dir
	    doCompile
	    cd ..
	done
    ;;

    
    batch)

	cd students

	for archive in *.tar.gz; do
	    export dir=$(echo $archive | sed "s/\.tar\.gz//g")
	    cd $dir
	    export firstname=$(echo $dir | cut -f 2 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
	    export lastname=$(echo $dir | cut -f 1 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
	    
	    echo "Batch processing $firstname $lastname..."
	    doFunctions
	    doModules
	    doAllInOne
	    doPreValgrind
	    doPreVariables
	    
	    cd ..
	done;
    ;;
    
    valgrind)
	cd students

	for archive in *.tar.gz; do
	    export dir=$(echo $archive | sed "s/\.tar\.gz//g")
	    cd $dir
	    export firstname=$(echo $dir | cut -f 2 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
	    export lastname=$(echo $dir | cut -f 1 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
    
	    echo "Interactive processing of valgrind log for $firstname $lastname..."
	    doValgrind
	    cd ..
	done
    ;;
    
    variables)
	cd students

	for archive in *.tar.gz; do
	    export dir=$(echo $archive | sed "s/\.tar\.gz//g")
	    cd $dir
	    export firstname=$(echo $dir | cut -f 2 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
	    export lastname=$(echo $dir | cut -f 1 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
    
	    echo "Interactive processing of variables of $firstname $lastname..."
	    doVariables
	    cd ..
	done
    ;;
    
    end)
	cd students
	for archive in *.tar.gz; do
	    export dir=$(echo $archive | sed "s/\.tar\.gz//g")
	    cd $dir
    	    export firstname=$(echo $dir | cut -f 2 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
	    export lastname=$(echo $dir | cut -f 1 -d @ | tr _ " " | tr "[:lower:]" "[:upper:]")
    
	    echo "Ending $firstname $lastname..."
	    doEnd
	    cd ..
	done
    ;;
    
    *)
	echo "You must specify prepare, batch, valgrind, variables or end as parameter"
    ;;
esac


