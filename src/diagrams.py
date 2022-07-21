#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May 24 15:44:58 2022

@author: tumpe
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt



def main():
    print(__doc__)
    
    
    
    #files=['CSbenchmark_AVX.csv','PSbenchmark_AVX.csv','RSbenchmark_AVX.csv']
    files=['CSbenchmark_AVX.csv','RSbenchmark_AVX.csv','CSbenchmark_Scalar.csv','RSbenchmark_Scalar.csv','RSbenchmark_Strided.csv']
    fontsize=25
    
    for f in files:
        df_tmp=pd.read_csv(f'../../output_files/{f}')
        if 'AVX' in f:
            df_tmp['Variant'] = np.repeat('AVX',len(df_tmp))
        elif 'Scalar' in f: 
            df_tmp['Variant'] = np.repeat('Scalar',len(df_tmp))
        else:
            df_tmp['Variant'] = np.repeat('Strided',len(df_tmp))
        if files.index(f)==0:
            df=df_tmp
        else:
            #df=df.append(df_tmp,ignore_index=True)
            df=pd.concat([df,df_tmp],ignore_index=True)
    
    print(df.head(-50))
    print(df.columns)
    plt.figure(figsize=(25,15))
    #pax_ratio=(df[(df[df.columns[0]]==2) & (df[df.columns[-1]]=='AVX')].values)[:,-2] / (df[(df[df.columns[0]]==2) & (df[df.columns[-1]]=='Scalar')].values)[:,-2]
    column_ratio=(df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='AVX')].values)[:,-2] / (df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='Scalar')].values)[:,-2]
    row_ratio=(df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='AVX')].values)[:,-2] / (df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='Scalar')].values)[:,-2]
    plt.plot((df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='AVX')].values)[:,1],column_ratio,ls='None',marker='x',ms=3,label='Column-Store')
    plt.hlines(y = 1, xmin = -1000, xmax = 60000, ls='dotted',colors='black')
    plt.xlim(0,50000)
    #plt.plot((df[(df[df.columns[0]]==2) & (df[df.columns[-1]]=='AVX')].values)[:,1],pax_ratio,ls='None',marker='x',ms=3,label='Pax-Store')
    plt.plot((df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='AVX')].values)[:,1],row_ratio,ls='None',marker='x',ms=3,label='Row-Store')
    plt.xlabel('rowCount',fontsize=fontsize)
    #plt.ylabel('CPU time [ms]',fontsize=fontsize)
    plt.tick_params(axis = 'both', which = 'major', labelsize = fontsize-3)
    plt.legend(loc='best',markerscale=4,fontsize=fontsize)
    plt.savefig('../../output_files/AVX-Scalar-ratio-50-cols-gt-ne-le-75k-2.pdf',dpi=300)
    
    plt.figure(figsize=(25,15))
    #column_ratio=(df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='AVX')].values)[:,-2] / (df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='Scalar')].values)[:,-2]
    #row_ratio=(df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='AVX')].values)[:,-2] / (df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='Scalar')].values)[:,-2]
    plt.plot((df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='AVX')].values)[:,1],(df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='AVX')].values)[:,-2],ls='None',marker='x',ms=3,label='Column-Store-AVX')
    plt.plot((df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='Scalar')].values)[:,1],(df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='Scalar')].values)[:,-2],ls='None',marker='x',ms=3,label='Column-Store-Scalar')
    plt.plot((df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='AVX')].values)[:,1],(df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='AVX')].values)[:,-2],ls='None',marker='x',ms=3,label='Row-Store-AVX')
    plt.plot((df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='Scalar')].values)[:,1],(df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='Scalar')].values)[:,-2],ls='None',marker='x',ms=3,label='Row-Store-Scalar')
    plt.plot((df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='Strided')].values)[:,1],(df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='Strided')].values)[:,-2],ls='None',marker='x',ms=3,label='Row-Store-Strided')
    #plt.plot((df[(df[df.columns[0]]==2) & (df[df.columns[-1]]=='AVX')].values)[:,1],(df[(df[df.columns[0]]==2) & (df[df.columns[-1]]=='AVX')].values)[:,-2],ls='None',marker='x',ms=3,label='PAX-Store-AVX')
    #plt.plot((df[(df[df.columns[0]]==2) & (df[df.columns[-1]]=='Scalar')].values)[:,1],(df[(df[df.columns[0]]==2) & (df[df.columns[-1]]=='Scalar')].values)[:,-2],ls='None',marker='x',ms=3,label='PAX-Store-Scalar')
    #plt.plot((df[(df[df.columns[0]]==1) & (df[df.columns[-1]]=='AVX')].values)[:,1],column_ratio,ls='None',marker='x',ms=3,label='Column-Store')
    #plt.plot((df[df[df.columns[0]]==2].values)[:,1],(df[df[df.columns[0]]==2].values)[:,-1],ls='None',marker='x',ms=3,label='Pax-Store')
    #plt.plot((df[df[df.columns[0]]==0].values)[:,1],(df[df[df.columns[0]]==0].values)[:,-1],ls='None',marker='x',ms=3,label='Row-Store')
    #plt.plot((df[(df[df.columns[0]]==0) & (df[df.columns[-1]]=='AVX')].values)[:,1],row_ratio,ls='None',marker='x',ms=3,label='Row-Store')
    #plt.ylim(0,50)
    plt.xlabel('rowCount',fontsize=fontsize)
    plt.ylabel('CPU time [ms]',fontsize=fontsize)
    plt.tick_params(axis = 'both', which = 'major', labelsize = fontsize-3)
    plt.legend(loc='best',markerscale=4,fontsize=fontsize)
    plt.savefig('../../output_files/AVX-Scalar-Strided-50-cols-gt-ne-le-75k.pdf',dpi=300)
    
    
    
if __name__ == "__main__":
    main()   