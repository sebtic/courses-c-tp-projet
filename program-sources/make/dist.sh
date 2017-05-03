#!/bin/bash

USER_FILES="src/main.c src/CatalogDB.c src/CatalogRecord.c src/CustomerDB.c src/CustomerRecord.c src/Dictionary.c src/Document.c src/DocumentRowList.c src/DocumentUtil.c src/EncryptDecrypt.c src/MyString.c src/OperatorTable.c src/PrintFormat.c"

rm -rf bindist
mkdir -p bindist

echo "Building analyzer..."
cd ../analyzer
 mvn clean install -q
cd -

echo "Building doc..."
rm -rf bindist/docsrc bindist/doc 
mkdir -p bindist/docsrc

cd include
for x in $(find . -type f | sed "s/\.\///g" | grep -v provided | grep -v user); do
    mkdir -p ../bindist/docsrc/$(dirname $x)
    cat $x | sed "s/OVERRIDABLE_PREFIX//g" | sed "s/OVERRIDABLE(\(.*\))(/\1(/g" > ../bindist/docsrc/$(basename $x)
done
cd ..
doxygen make/Doxyfile 

#echo "Building subject..."
#cd sujet
#make clean all
#cd ..

#echo "Building courses..."
#cd courses
#make clean all
#cd ..

#echo "Building moniteur..."
#cd moniteur
#make clean all
#cd ..


echo "Assembling files..."
rm -rf bindist/forstudent
mkdir bindist/forstudent
cp -r include bindist/forstudent/
cp -r src bindist/forstudent/
cp -r printformat bindist/forstudent/
cp -r provided bindist/forstudent/
mkdir -p bindist/forstudent/make
cp make/main.mk bindist/forstudent/make/
cp make/main-obj.mk bindist/forstudent/make/
cp make/package.mk bindist/forstudent/make/
cp make/package.sh bindist/forstudent/make/
cp make/valgrind.mk bindist/forstudent/make/
cp make/valgrind-suppressions.txt bindist/forstudent/make/
cp ../analyzer/target/analyzer-simple.jar bindist/forstudent/make/
cp make/project.cbp bindist/forstudent/
echo "include make/main.mk" > bindist/forstudent/Makefile
echo "include make/main-obj.mk" >> bindist/forstudent/Makefile
echo "include make/package.mk" >> bindist/forstudent/Makefile
echo "include make/valgrind.mk" >> bindist/forstudent/Makefile
mkdir -p bindist/forstudent/doc
cp -r doc/html/* bindist/forstudent/doc/
#mkdir -p bindist/forstudent/subject bindist/forstudent/courses 
#mkdir -p bindist/forstudent/moniteur
#cp sujet/dist/TDsTPs-student.pdf bindist/forstudent/subject/
#cp courses/dist/courses-student-print.pdf bindist/forstudent/courses/
#cp moniteur/dist/moniteur.pdf bindist/forstudent/moniteur/


arch=$(uname -m)
mkdir -p bindist/toupload

cd bindist/forstudent

echo "Building doc.tar.gz..."
#tar -czf ../toupload/doc.tar.gz doc subject courses moniteur
tar -czf ../toupload/doc.tar.gz doc 
#rm -rf doc subject courses moniteur
rm -rf doc 

echo "Assembling user-$arch.tar.gz..."
tar -czf ../toupload/user-$arch.tar.gz ${USER_FILES}
rm ${USER_FILES}

echo "Assembling base-$arch.tar.gz"
tar -czf ../toupload/base-$arch.tar.gz . 

cd ../
rm -rf forstudent docsrc doc 


kdialog --title "Upload files to the server ?" --yesno "Upload files to the server ?"
if [[ "$?" = "0" ]]; then
    ssh root@storage.projectsforge.org "lxc exec storage -- mkdir -p /var/www/storage/htdocs/update/c/"
    for f in toupload/doc.tar.gz toupload/user-$arch.tar.gz toupload/base-$arch.tar.gz ../make/update.sh ../make/update-sub.sh; do
	cat $f | ssh root@storage.projectsforge.org "lxc exec storage -- bash -c 'dd of=/var/www/storage/htdocs/update/c/$(basename $f)'" 
    done
    ssh root@storage.projectsforge.org "lxc exec storage -- chown -R www-data:www-data /var/www/storage/htdocs/"
fi