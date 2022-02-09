%%
%CODIGO DESENVOLVIDO POR:
%Edgar Filipe Ferreira Duarte
%Pedro Guilherme da Cruz Ferreira
%DEI 2020/2021

function [freqs,dft] = dftCalc(data, fs)
    
    %Function created in class. Calculates the dft. If the number of
    %elements is a uneven value then we have to make the frequencies a
    %half a unit larger so that when we apply the fftshift to the discrete
    %Fourier tranform (DFT)
    
    N = numel(data);
    if (mod(N,2)==0)
        freqs = -fs/2:fs/N:fs/2-fs/N;
    else
        freqs = -fs/2+fs/(2*N):fs/N:fs/2-fs/(2*N);
    end
   
    
    %disp(data);
    X = fftshift(fft(data));

    %remove residue
    
    
    dft = abs(X);
    dft(abs(dft)<0.001) = 0;

    
end