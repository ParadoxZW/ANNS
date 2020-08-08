% m = [];
% for i=1:16
%     t = centers_table_opq_np{i};
%     mm = t(code_opq_np(1, i), :);
%     m = [m mm];
% end
% xx = Xtrain(1,:)*R_opq_np;
% norm(m - xx)
% fvecs_write('R.fvecs', R_opq_np);
% for i=1:M
%     fvecs_write(['c' num2str(i) '.fvecs'], centers_table_opq_np{i});
% end
cvecs_write('code.cvecs', code_opq_np);