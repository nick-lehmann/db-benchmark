#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May 24 15:44:58 2022

@author: tumpe
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import glob
from random import randint

def get_cmap(n, name='hsv'):
    return plt.cm.get_cmap(name, n)


def main():
    print(__doc__)
    
    #files=['CSbenchmark_AVX.csv','PSbenchmark_AVX.csv','RSbenchmark_AVX.csv']
    #files=['CSbenchmark_AVX.csv','RSbenchmark_AVX.csv','CSbenchmark_Scalar.csv','RSbenchmark_Scalar.csv','RSbenchmark_Strided.csv']
    files=list(glob.glob('../../output_files/*.csv'))
    files_rows=[f for f in files if 'fNR' not in f]
    files_filter=[f for f in files if 'fNR' in f]
    #print(files)
    fontsize=25
    
    for f in files_rows:
        df_tmp=pd.read_csv(f)
        if 'AVX' in f:
            if '32' in f:
                df_tmp['Variant'] = np.repeat('AVX32',len(df_tmp))
            else:
                df_tmp['Variant'] = np.repeat('AVX64',len(df_tmp))
        elif 'Scalar' in f: 
            if '32' in f:
                df_tmp['Variant'] = np.repeat('Scalar32',len(df_tmp))
            else:
                df_tmp['Variant'] = np.repeat('Scalar64',len(df_tmp))
        else:
            if '32' in f:
                df_tmp['Variant'] = np.repeat('Strided32',len(df_tmp))
            else:
                df_tmp['Variant'] = np.repeat('Strided64',len(df_tmp))
        if files_rows.index(f)==0:
            df=df_tmp
        else:
            #df=df.append(df_tmp,ignore_index=True)
            df=pd.concat([df,df_tmp],ignore_index=True)
            
            
    for f in files_filter:
        df_tmp=pd.read_csv(f)
        if 'AVX' in f:
            if '32' in f:
                df_tmp['Variant'] = np.repeat('AVX32',len(df_tmp))
            else:
                df_tmp['Variant'] = np.repeat('AVX64',len(df_tmp))
        elif 'Scalar' in f: 
            if '32' in f:
                df_tmp['Variant'] = np.repeat('Scalar32',len(df_tmp))
            else:
                df_tmp['Variant'] = np.repeat('Scalar64',len(df_tmp))
        else:
            if '32' in f:
                df_tmp['Variant'] = np.repeat('Strided32',len(df_tmp))
            else:
                df_tmp['Variant'] = np.repeat('Strided64',len(df_tmp))
        df_tmp['fNR'] = np.repeat(int(f.split('_')[-1][:-4]),len(df_tmp))
        if files_filter.index(f)==0:
            df_filter=df_tmp
        else:
            #df=df.append(df_tmp,ignore_index=True)
            df_filter=pd.concat([df_filter,df_tmp],ignore_index=True)
    
    print(df_filter.head(-50))
    print(df_filter.columns)
    
    
    labels=['Row-Store','Column-Store','PAX-Store']
    simds=['AVX','Scalar','Strided']
    marker=['^','X','D']
    dtype_idx={'32':0,'64':1}
    colors = matplotlib.cm.tab10([0,1,2,3,4,5,7,8]).tolist()
    # marker size:
    ms=5
    #rng = np.random.default_rng(42)
    #rng.shuffle(colors)
    #colors_dtype={'32':'b','64':'r'}
    #colors=get_cmap(14)
    #colors=['#e6194B', '#3cb44b', '#ffe119', '#4363d8', '#469990','#911eb4','#808000', 
    #        '#dcbeff', '#9A6324', '#800000', '#aaffc3', '#000075', '#a9a9a9', '#000000']

    colors.append('#000075')
    colors.append('#ffe119')
    colors.append('#911eb4')
    colors.append('#000000')
    #rng = np.random.default_rng(14)
    #rng.shuffle(colors)
    colors_column=np.array(colors).reshape((3,4))
    figsize=(25,15)
    
    """ Speedup Diagram """
    fig, axs = plt.subplots(1, 2,figsize=figsize,sharey=True)
    axs=axs.flatten()
    plt.figure(figsize=figsize)
    for t in ['32','64']:
        for i in range(3):
            ratio=(df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[1]}{t}')].values)[:,-2] / (df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[0]}{t}')].values)[:,-2]
            #plt.plot((df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[1]}{t}')].values)[:,1],ratio,ls='None',marker=marker[i],c=f'{"#%06X" % randint(0, 0xFFFFFF)}',ms=4,label=f'{labels[i]}-{t}')
            #plt.plot((df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[1]}{t}')].values)[:,1],ratio,ls='None',marker=marker[i],c=colors[c],ms=7,label=f'{labels[i]}-{t}')
            axs[dtype_idx[t]].plot((df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[1]}{t}')].values)[:,1],ratio,ls='None',
                                   marker=marker[i],c=colors[i],ms=ms,label=f'{labels[i]}')
            if i==0:
                ratio=(df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[1]}{t}')].values)[:,-2] / (df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[2]}{t}')].values)[:,-2]
                axs[dtype_idx[t]].plot((df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[1]}{t}')].values)[:,1],
                         ratio,ls='None',marker='s',c=colors[-1],ms=ms,label=f'{labels[i]}-Strided')  
                
                
                #f'{"#%06X" % randint(0, 0xFFFFFF)}'
        axs[dtype_idx[t]].set_xlabel('# rows',fontsize=fontsize-3)
        axs[dtype_idx[t]].set_title(f'uint{t}_t',fontsize=fontsize-1)
        axs[dtype_idx[t]].hlines(y = 1, xmin = -1000, xmax = 60000,linewidths=3, ls='dotted',colors='black')
        axs[dtype_idx[t]].tick_params(axis = 'both', which = 'major', labelsize = fontsize-2)
        #axs[dtype_idx[t]].set_xlim(0,50000)
        #axs[dtype_idx[t]].set_ylim(0,8)
        axs[dtype_idx[t]].set_ylabel('Speedup AVX512 vs Scalar',fontsize=fontsize-3)
    axs[dtype_idx[t]].legend(loc='best',markerscale=4,fontsize=fontsize)
    #plt.ylabel('CPU time [ms]',fontsize=fontsize)
    fig.tight_layout(rect=[0, 0.03, 1.0, 0.95])
    #plt.tight_layout(rect=[0, 0.03, 1.0, 0.95])
    fig.savefig('../../output_files/AVX-Speedup-50-cols-6-filter-4gb.pdf',dpi=300)
    

    
    """ Speed Diagram  """
    fig, axs = plt.subplots(3, 2,figsize=figsize,sharex=True)
    tp_dict={'32':2,'64':4}
    #axs=axs.flatten()
    for i in range(3):
        axs[i,dtype_idx['32']].set_ylabel('Throughput [GB / s]',fontsize=fontsize-5)
        for t in ['32','64']:
            for s in simds[:2]:
                ts=np.array((df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{s}{t}')].values)[:,-2])
                rc=np.array((df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{s}{t}')].values)[:,1])
                y=tp_dict[t]*rc/(int(1e+4)*ts)
                axs[i,dtype_idx[t]].plot(rc,y,ls='None',c=colors_column[i,simds.index(s)],marker=marker[i],ms=ms,label=f'{labels[i]}-{s}')   
            if i==0:
                #for t in ['32','64']:
                rc=(df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[2]}{t}')].values)[:,1]
                ts=(df[(df[df.columns[0]]==i) & (df[df.columns[-1]]==f'{simds[2]}{t}')].values)[:,-2]
                y=tp_dict[t]*rc/(int(1e+4)*ts)
                axs[i,dtype_idx[t]].plot(rc,y,ls='None',c=colors_column[i,2],marker='s',ms=ms,label=f'{labels[i]}-Strided')
                
                axs[i,dtype_idx[t]].set_title(f'uint{t}_t',fontsize=fontsize-1)
            axs[i,dtype_idx[t]].set_xlabel('# rows',fontsize=fontsize-3)
            
            #axs[i,dtype_idx[t]].set_ylabel('Million Integer / s',fontsize=fontsize-2)
            axs[i,dtype_idx[t]].tick_params(axis = 'both', which = 'major', labelsize = fontsize-4)
        axs[i,dtype_idx[t]].legend(loc='best',markerscale=3,fontsize=fontsize)
        #axs[i,dtype_idx[t]].legend(loc='best',markerscale=4,fontsize=fontsize-4)
    #plt.ylim(0,50)
    
    #plt.legend(loc='best',markerscale=4,fontsize=fontsize)
    fig.tight_layout(rect=[0, 0.03, 1.0, 0.95])
    fig.savefig('../../output_files/AVX-Scalar-Strided-50-cols-6-filter-4gb.pdf',dpi=300)
    
    """ Filter Diagram """
    fig, axs = plt.subplots(3, 2,figsize=figsize,sharex=True)
    for i in range(3):
        for t in ['32','64']:
            for s in simds[:2]:
                axs[i,dtype_idx[t]].plot((df_filter[(df_filter[df_filter.columns[0]]==i) & (df_filter[df_filter.columns[-2]]==f'{s}{t}')].values)[:,-1],
                                         (df_filter[(df_filter[df_filter.columns[0]]==i) & (df_filter[df_filter.columns[-2]]==f'{s}{t}')].values)[:,-3],
                                         ls='None',marker=marker[i],c=colors_column[i,simds.index(s)],ms=ms,label=f'{labels[i]}-{s}')
            axs[i,dtype_idx[t]].set_ylabel('CPU time [ms]',fontsize=fontsize-2)
            axs[i,dtype_idx[t]].set_xlabel('# filter',fontsize=fontsize-2)
            axs[i,dtype_idx[t]].tick_params(axis = 'both', which = 'major', labelsize = fontsize-4)
            if(i==0):
                axs[i,dtype_idx[t]].set_title(f'uint{t}_t',fontsize=fontsize-1)
        axs[i,dtype_idx[t]].legend(loc='best',markerscale=4,fontsize=fontsize)
    fig.tight_layout(rect=[0, 0.03, 1.0, 0.95])
    fig.savefig('../../output_files/Filter.pdf',dpi=300)
    
    
    
if __name__ == "__main__":
    main()   