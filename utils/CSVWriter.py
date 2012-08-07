#!/usr/bin/python
#-*- coding: utf-8 -*-
'''
Created on 03-08-2012

@author: jezozwierzak
'''

class CSVWriter:
        
    def __init__(self, separator = ';'):
        self.widths = []
        self.list  = []
        self.rows = 0
        self.columns = 0
        self.separator = separator
    
    def addSubColumn(self, parentIndex, name):
        parentColumn = self.list[parentIndex];
        parentColumn.append([name])
        
        subColsStr = ''
        for i in range(1,len(parentColumn)):
            subColsStr += parentColumn[i][0] + self.separator
            
        if len(subColsStr) > self.widths[parentIndex]:
            self.widths[parentIndex] = len(subColsStr) - 1
        
        if len(parentColumn[1:]) > 1:
            self.columns+=1
    
    def addSubColumnByName(self, parentName, name):
        assert self.rows == 0, 'You have to add all Column names before adding rows'
        parentIndex = self.columnIndex(parentName)
        self.addSubColumn(parentIndex, name)
    
    def addSubColumnsByName(self, parentName, names=[]):
        for name in names:
            self.addSubColumnByName(parentName, name)
    
    def addSubColumns(self, parentIndex, names=[]):
        for name in names:
            self.addSubColumn(parentIndex, name)
    
    def addColumn(self, name):
        assert self.rows == 0, 'You have to add all Column names before adding rows'
        self.list.append([name])
        self.widths.append(len(name))
        self.columns+=1
    
    def addColumns(self, names=[]):
        for name in names:
            self.addColumn(name)
    
    def addRow(self, row=[]):
        assert len(row) == len(self.list), 'Wrong number of columns in row'
        
        for i in range(0,len(self.list)):
            column = self.list[i]
            if len(column) > 1 and type(column[1]).__name__ == 'list':
                #Adding data to subcolumns
                assert len(row[i]) == len(column) - 1, 'Wrong number of subColumns in column ' + column[0]
                
                for j in range(0,len(row[i])):
                    column[j+1].append(row[i][j])

                subColsStr = ''
                for j in range(0,len(row[i])):
                    subColsStr += '{0:{base}}'.format(row[i][j], base='.4f') + self.separator + ' '
                
                if len(subColsStr) > self.widths[i]:
                    self.widths[i] = len(subColsStr) - 1
            else:
                #Adding data to column
                column.append(row[i])
                
                if len(str(row[i])) > self.widths[i]:
                    self.widths[i] = len(str(row[i]))
                    
        self.rows+=1           

    def allWidth(self):
        sum = 0
        for width in self.widths:
            sum += width
        return width
    
    def columnIndex(self, name):
        for column in self.list:
            if column[0] == name:
                return self.list.index(column)
    
    def hasSubColumns(self):
        for column in self.list:
            if len(column) > 1 and type(column[1]).__name__ == 'list':
                return True
        return False
    
    def hasColumnSubColumns(self, index):
        column = self.list[index]
        return len(column) > 1 and type(column[1]).__name__ == 'list'
        
    def repeat_to_length(self, string_to_expand, length):
        return (string_to_expand * ((length/len(string_to_expand))+1))[:length]

    def count_avg(self, ):
        results = [] 
        if not self.hasSubColumns():
            for i in range(0,len(self.list)): #Iterowanie po kolumnach
                results.append(0)
                for j in range(1,1+self.rows): # Iterowanie po wierszach
                    results[i]+= self.list[i][j]
                results[i]/=self.rows
        else:
            for i in range(0,len(self.list)): #Iterowanie po kolumnach
                
                if self.hasColumnSubColumns(i):
                    subResults = []
                    for k in range(0,len(self.list[i][1:])):
                        subColumn = self.list[i][1:][k]
                        subResults.append(0)
                        for j in range(1,1+self.rows): # Iterowanie po wierszach
                            subResults[k]+= subColumn[j]
                        subResults[k]/=self.rows
                    results.append(subResults)
                else:
                    results.append(0)
                    for j in range(1,1+self.rows): # Iterowanie po wierszach
                        results[i]+= self.list[i][j]
                    results[i]/=self.rows
        results = results[1:]
        results[:0] = ['AVG']
        self.addRow(results)

    def __str__(self):
        result = ''
        if not self.hasSubColumns():
            for j in range(0,1+self.rows): # Iterowanie po wierszach
                for i in range(0, len(self.list)): #Iterowanie po kolumnach
                    if type(self.list[i][j]).__name__=='int':
                        result += '{0:{width}{base}}'.format(self.list[i][j], base='d', width=self.widths[i]) + self.separator
                    elif type(self.list[i][j]).__name__=='float':
                        result += '{0:{width}{base}}'.format(self.list[i][j], base='.4f', width=self.widths[i]) + self.separator
                    else:
                        result += '{0:{width}{base}}'.format(self.list[i][j], base='s', width=self.widths[i]) + self.separator
                result += '\n'
        else:
            #Printing Thead
            thead = zip(*self.list)[0]
            for i in range(0,len(thead)):
                if self.hasColumnSubColumns(i):
                    numberOfColumns = len(self.list[i][1:])
                    result += '{0:{width}{base}}'.format(thead[i], base='s', width=self.widths[i]-numberOfColumns+1) + self.separator
                    for j in range(1,numberOfColumns):
                        result += self.separator
                else:
                    result += '{0:{width}{base}}'.format(thead[i], base='s', width=self.widths[i]) + self.separator
            result += '\n'
            #Printing subTheads:
            for i in range(0,len(self.list)):
                if self.hasColumnSubColumns(i):
                    numberOfColumns = len(self.list[i][1:])
                    for subColumn in self.list[i][1:]:
                        result += '{0:{width}{base}}'.format(subColumn[0], base='s', width=(self.widths[i]/numberOfColumns)) + self.separator
                else:
                    result += '{0:{width}{base}}'.format('', base='s', width=self.widths[i]) + self.separator
            result += '\n'
            #Printing Data

            for j in range(1,1+self.rows): # Iterowanie po wierszach
                for i in range(0, len(self.list)): #Iterowanie po kolumnach
                    if self.hasColumnSubColumns(i):
                        
                        for subcolumns in self.list[i][1:]:
                            if type(subcolumns[j]).__name__=='int':
                                result += '{0:{width}{base}}'.format(subcolumns[j], base='d', width=(self.widths[i]/numberOfColumns)) + self.separator
                            elif type(subcolumns[j]).__name__=='float':
                                result += '{0:{width}{base}}'.format(subcolumns[j], base='.4f', width=(self.widths[i]/numberOfColumns)) + self.separator
                            else:
                                result += '{0:{width}{base}}'.format(subcolumns[j], base='s', width=(self.widths[i]/numberOfColumns)) + self.separator
                    else:
                        
                        if type(self.list[i][j]).__name__=='int':
                            result += '{0:{width}{base}}'.format(self.list[i][j], base='d', width=self.widths[i]) + self.separator
                        elif type(self.list[i][j]).__name__=='float':
                            result += '{0:{width}{base}}'.format(self.list[i][j], base='.4f', width=self.widths[i]) + self.separator
                        else:
                            result += '{0:{width}{base}}'.format(self.list[i][j], base='s', width=self.widths[i]) + self.separator
                result += '\n'
        return result


