function [] = plot_laplace(mu,scale)

  fig = figure();
  hold on;
  set(gcf, 'Color', 'w');
  %set(gca,'xtick',[]);
  set(gca,'xticklabel',[]);
  set(gca,'ytick',[]);
  %set(gca,'yticklabel',[]);
  %set(gca, 'visible', 'off');

  for x=mu-5:0.01:mu+5
    diff = x-mu;
    y = (0.5/scale) * exp(-abs(diff)/scale);
    plot(x,y,'k.');
  end

  set(gca,'Ylim',[0 0.5]);
  savefig(fig,'laplace');

end

