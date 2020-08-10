import os
import time
import numpy as np
import faiss

def ivecs_read(fname):
    a = np.fromfile(fname, dtype="int32")
    d = a[0]
    return a.reshape(-1, d + 1)[:, 1:].copy()

def fvecs_read(fname):
    return ivecs_read(fname).view('float32')

def fvecs_write(x, fname):
    d, w = x.shape
    # print(w)
    x = x.view('int32')
    y = np.ones((d, 1), dtype='int32') * w
    x = np.concatenate([y, x], -1).reshape(-1)
    x.tofile(fname)

def cvecs_write(x, fname):
    x = x.astype('uint8')
    x.tofile(fname)

x = fvecs_read("sift/sift_base.fvecs")
# x = x[:10000, :]
n, d = x.shape
m = 8
opq = faiss.OPQMatrix(d, 8)
# help(opq)
opq.train(x)
A = faiss.vector_to_array(opq.A).reshape(d, d)
print(A.shape)
# print(A)
xt = opq.apply_py(x)
# print(((np.dot(x[0], A.T) - xt[0])**2).sum())
# print(x[0, :10])
# print(xt[0, :10])
print(xt.shape)
pq = faiss.ProductQuantizer(d, 8, 8)
pq.train(xt)
codes = pq.compute_codes(x)
cen = faiss.vector_to_array(pq.centroids)
cen = cen.reshape(pq.M, pq.ksub, pq.dsub)
print(cen.shape)
print(codes.shape)
# print(cen[0][codes[0, 0]][:10])
fvecs_write(A, 'opq/R.fvecs')
for i in range(m):
    fvecs_write(cen[i], 'opq/c'+str(i)+'.fvecs')
cvecs_write(codes, 'opq/code.cvecs')
